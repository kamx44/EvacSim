#include "Contact.h"
#include "Object.h"
#include "Mmath.h"
#include <GL/gl.h>

// HACK : use a shared material for all objects
//Material s_xContactMaterial;

#define dbg_UseImpulses 0
#define dbg_UseFriction 1

#define GetStaticFriction 0.3
#define GetSeparation 0.1
#define GetRestitution 0.7
#define GetFriction 0.2

void ResolveOverlap(  const glm::vec2& Ncoll, float depth, float spring_value, float spring_damper,
					  const glm::vec2& C0, const glm::vec2& P0, glm::vec2& V0, float w0,
					  const glm::vec2& C1, const glm::vec2& P1, glm::vec2& V1, float w1,
					  glm::vec2& Fspring)
{
	glm::vec2 R0    = C0 - P0;
	glm::vec2 R1    = C1 - P1;
	glm::vec2 T0    = glm::vec2(-R0.y, R0.x);
	glm::vec2 T1    = glm::vec2(-R1.y, R1.x);
	glm::vec2 VP0   = V0 - T0 * w0; // point velocity (SIGN IS WRONG)
	glm::vec2 VP1   = V1 - T1 * w1; // point velocity (SIGN IS WRONG)

	glm::vec2 D = C0 - C1;
	glm::vec2 V = VP0 - VP1;

	float  vn = glm::dot(V, Ncoll);
	float  dn = glm::dot(D, Ncoll);

//	if (dn > 0.0f) dn = 0.0f;

	float  f = (spring_value * dn) - (spring_damper * vn);

	f = clampf(f, -1000.0f, 1000.0f);
	Fspring = -f * Ncoll;
}



////////////////////////////////////////////////////////////////
// ResolveCollision
////////////////////////////////////////////////////////////////
// calculates the change in angular and linear velocity of two
// colliding objects
////////////////////////////////////////////////////////////////
// parameters :
// ------------
// Ncoll : normal of collision
// t : time of collision, (if negative, it's a penetration depth)
// fCoF : coefficient of friction
// fCoR : coefficient of restitution
// C0 : point of contact on object 0
// P0 : centre of gravity (position) of object 0
// V0 : linear Velocity of object 0
// w0 : angular velocity of object 0
// m0 : inverse mass of object 0
// i0 : inverse inertia of object 0
// C1 : point of contact on object 1
// P1 : centre of gravity (position) of object 1
// V1 : linear Velocity of object 1
// w1 : angular velocity of object 1
// m1 : inverse mass of object 1
// i1 : inverse inertia of object 1
//
// return values : V0, w0, V1, w1 will change upon a collision
// -------------
///////////////////////////////////////////////////////////////
void ResolveCollision(const glm::vec2& Ncoll, float t, float fCoF, float fCoR,
					  const glm::vec2& C0, const glm::vec2& P0, glm::vec2& V0, float& w0, float m0, float i0,
					  const glm::vec2& C1, const glm::vec2& P1, glm::vec2& V1, float& w1, float m1, float i1)
{
	//------------------------------------------------------------------------------------------------------
	// pre-computations
	//------------------------------------------------------------------------------------------------------
	float tcoll = (t > 0.0f)? t : 0.0f;

	glm::vec2 Q0    = P0 + V0 * tcoll;
	glm::vec2 Q1    = P1 + V1 * tcoll;
	glm::vec2 R0    = C0 - Q0;
	glm::vec2 R1    = C1 - Q1;
	glm::vec2 T0    = glm::vec2(-R0.y, R0.x);
	glm::vec2 T1    = glm::vec2(-R1.y, R1.x);
	glm::vec2 VP0   = V0 - T0 * w0; // point velocity (SIGN IS WRONG)
	glm::vec2 VP1   = V1 - T1 * w1; // point velocity (SIGN IS WRONG)

	//------------------------------------------------------------------------------------------------------
	// impact velocity
	//------------------------------------------------------------------------------------------------------
	glm::vec2 Vcoll = VP0 - VP1;
	float  vn	 = glm::dot(Vcoll, Ncoll);
	glm::vec2 Vn	 = vn    * Ncoll;
	glm::vec2 Vt	 = Vcoll - Vn;

	// separation
	if (vn > 0.0f)
		return;

	float  vt = glm::length(Vt);


	//------------------------------------------------------------------------------------------------------
	// compute impulse (frction and restitution).
	// ------------------------------------------
	//
	//									-(1+Cor)(Vel.norm)
	//			j =  ------------------------------------------------------------
	//			     [1/Ma + 1/Mb] + [Ia' * (ra x norm)²] + [Ib' * (rb x norm)²]
	//------------------------------------------------------------------------------------------------------
	glm::vec2 J;
	glm::vec2 Jt(0.0f, 0.0f);
	glm::vec2 Jn(0.0f, 0.0f);

	float t0 = cross(R0, Ncoll) * cross(R0, Ncoll) * i0;
	float t1 = cross(R1, Ncoll) * cross(R1, Ncoll) * i1;
	float m  = m0 + m1;

	float denom = m + t0 + t1;

	float jn = vn / denom;

	Jn = (-(1.0f + fCoR) * jn) * Ncoll;

	if (dbg_UseFriction)
	{
		Jt = (fCoF * jn) * glm::normalize(Vt);
	}

	J = Jn + Jt;

	//------------------------------------------------------------------------------------------------------
	// changes in momentum
	//------------------------------------------------------------------------------------------------------
	glm::vec2 dV0 = J * m0;
	glm::vec2 dV1 =-J * m1;

	float dw0 =-cross(R0, J) * i0; // (SIGN IS WRONG)
	float dw1 = cross(R1, J) * i1; // (SIGN IS WRONG)

	//------------------------------------------------------------------------------------------------------
	// apply changes in momentum
	//------------------------------------------------------------------------------------------------------
	if (m0 > 0.0f) V0 += dV0;
	if (m1 > 0.0f) V1 += dV1;
	if (m0 > 0.0f) w0 += dw0;
	if (m1 > 0.0f) w1 += dw1;

	//------------------------------------------------------------------------------------------------------
	// Check for static frcition
	//------------------------------------------------------------------------------------------------------
	if (vn < 0.0f && fCoF > 0.0f)
	{
		float cone = -vt / vn;

		if (cone < fCoF)
		{
			// treat static friciton as a collision at the contact point
			glm::vec2 Nfriction = glm::normalize(-Vt);
			float fCoS = GetStaticFriction;

			ResolveCollision(Nfriction, 0.0f, 0.0f, fCoS,
							 C0, P0, V0, w0, m0, i0,
							 C1, P1, V1, w1, m1, i1);
		}
	}
}

Contact::Contact()
{
	Reset();
}

void Contact::Reset()
{
	m_pxBodies[0] = m_pxBodies[1] = NULL;
	m_iNumContacts = 0;
}
Contact::Contact(const glm::vec2* CA, const glm::vec2* CB, int Cnum,
				   const glm::vec2& N, float t,
				   Object* pxBodyA, Object* pxBodyB)
{
	m_iNumContacts = 0;
	m_pxBodies[0]  = pxBodyA;
	m_pxBodies[1]  = pxBodyB;
	m_xNormal	   = N;
	m_t			   = t;

	for(int i = 0; i < Cnum; i ++)
	{
		AddContactPair(CA[i], CB[i]);
	}
}

void Contact::Render(void) const
{
	for(int i = 0; i < m_iNumContacts; i ++)
	{
		glBegin(GL_LINES);
		glColor4f(1.0f, 0.3f, 0.3f, 1.0f);
		glVertex2fv(&m_xContacts[i][0].x);
		glVertex2fv(&m_xContacts[i][1].x);
		glEnd();

		glBegin(GL_POINTS);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertex2fv(&m_xContacts[i][0].x);
		glVertex2fv(&m_xContacts[i][1].x);
		glEnd();
	}
}

void Contact::AddContactPair(const glm::vec2& CA, const glm::vec2& CB)
{
	if (m_iNumContacts >= eMaxContacts)
		return;

	m_xContacts[m_iNumContacts][0] = CA;
	m_xContacts[m_iNumContacts][1] = CB;
	m_iNumContacts++;
}

void Contact::Solve()
{
	if (m_t < 0.0f)
		ResolveOverlap();

	ResolveCollision();
}

void Contact::ResolveOverlap()
{
	if (!m_pxBodies[0] || !m_pxBodies[1])
		return;

	for(int i = 0; i < m_iNumContacts; i ++)
	{
		ResolveOverlap(m_xContacts[i][0], m_xContacts[i][1]);
	}
}

void Contact::ResolveOverlap(const glm::vec2& C0, const glm::vec2& C1)
{
/*	glm::vec2 Fspring;
	float spring_value = 10000.0f;
	float spring_damper = 100.0f;

	const glm::vec2& P0	= m_pxBodies[0]->GetPosition();
	const glm::vec2& P1	= m_pxBodies[1]->GetPosition();
	glm::vec2& V0			= m_pxBodies[0]->GetLinVelocity();
	glm::vec2& V1			= m_pxBodies[1]->GetLinVelocity();
	float&  w0			= m_pxBodies[0]->GetAngVelocity();
	float&  w1			= m_pxBodies[1]->GetAngVelocity();

	::ResolveOverlap(m_xNormal, m_t, spring_value, spring_damper,
					 C0, P0, V0, w0,
					 C1, P1, V1, w1,
					 Fspring);

	m_pxBodies[0]->AddForce(C0, Fspring);
	m_pxBodies[1]->AddForce(C1,-Fspring);

	return;
	*/
	float m0 = m_pxBodies[0]->inverseMass;
	float m1 = m_pxBodies[1]->inverseMass;
	float m  = m0 + m1;

	glm::vec2 D = C1 - C0;
	float fRelaxation = GetSeparation;

	D *= fRelaxation;
	glm::vec2 D0 = glm::vec2(0.0f, 0.0f);
	glm::vec2 D1 = glm::vec2(0.0f, 0.0f);

	if (m0 > 0.0f)
	{
		D0 = D * (m0 / m);
		m_pxBodies[0]->position += D0;
	}
	if (m1 > 0.0f)
	{
		D1 = D * -(m1 / m);
		m_pxBodies[1]->position += D1;
	}
}

void Contact::ResolveCollision()
{
	if (!m_pxBodies[0] || !m_pxBodies[1])
		return;

	for(int i = 0; i < m_iNumContacts; i ++)
	{
		ResolveCollision(m_xContacts[i][0], m_xContacts[i][1]);
	}
}




void Contact::ResolveCollision(const glm::vec2& C0, const glm::vec2& C1)
{
	float m0 = m_pxBodies[0]->inverseMass;
	float m1 = m_pxBodies[1]->inverseMass;
	float i0 = m_pxBodies[0]->inverseInertia;
	float i1 = m_pxBodies[1]->inverseInertia;

	const glm::vec2& P0	= m_pxBodies[0]->position;
	const glm::vec2& P1	= m_pxBodies[1]->position;
	glm::vec2& V0			= m_pxBodies[0]->velocity;
	glm::vec2& V1			= m_pxBodies[1]->velocity;
	float&  w0			= m_pxBodies[0]->angularVelocity;
	float&  w1			= m_pxBodies[1]->angularVelocity;

//	float fCoR  = GetRestitution;
//	float fCoF  = GetFriction;

	::ResolveCollision(-m_xNormal, m_t, GetFriction, GetRestitution,
					   C1, P1, V1, w1, m1, i1,
					   C0, P0, V0, w0, m0, i0);

//	Render();
}
