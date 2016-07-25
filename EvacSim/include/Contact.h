#ifndef CONTACT_H
#define CONTACT_H
#include "Object.h"


class Contact
{
public:
	enum { eMaxContacts = 2 };

	class Object* m_pxBodies [2];
	glm::vec2       m_xContacts[eMaxContacts][2];
	glm::vec2		 m_xNormal;
	float		 m_t;
	int			 m_iNumContacts;

	Contact();

	Contact(const glm::vec2* CA, const glm::vec2* CB, int iCnum,
			 const glm::vec2& N, float t,
			 Object* pxBodyA, Object* pxBodyB);

	void Reset();

	void Render(void) const;

	class Object* GetBody(int i) { return m_pxBodies[i]; }

	void Solve();

private:
	void ResolveCollision();
	void ResolveOverlap  ();

	void ResolveCollision(const glm::vec2& CA, const glm::vec2& CB);
	void ResolveOverlap  (const glm::vec2& CA, const glm::vec2& CB);
	void AddContactPair	 (const glm::vec2& CA, const glm::vec2& CB);
};

#endif // CONTACT_H
