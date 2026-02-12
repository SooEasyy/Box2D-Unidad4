#include "ContactListener.h"
#include <iostream>

void ContactListener::BeginContact(b2Contact* contact)

{
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();

    std::cout
        << "A type: " << bodyA->GetUserData().pointer
        << " | B type: " << bodyB->GetUserData().pointer
        << std::endl;

    uintptr_t typeA = bodyA->GetUserData().pointer;
    uintptr_t typeB = bodyB->GetUserData().pointer;

    bool AisGoal = (typeA == 2);
    bool BisGoal = (typeB == 2);

    bool AisRagdoll = (typeA == 1);
    bool BisRagdoll = (typeB == 1);

    // SOLO si un ragdoll toca un goal → ganar nivel
    if (levelAlreadyWon)
        return;

    if ((AisGoal && BisRagdoll) || (BisGoal && AisRagdoll))
    {
        levelAlreadyWon = true;
        if (onGoalReached)
            onGoalReached();
    }
}
