//
//  MyContactListener.cpp
//  Breakout
//
//  Created by Augusto Souza on 8/24/11.
//  Copyright 2011 Menki Mobile. All rights reserved.
//

#include "MyContactListener.h"

MyContactListener::MyContactListener() : contacts() 
{
}

MyContactListener::~MyContactListener() 
{
}

void MyContactListener::BeginContact(b2Contact* contact)
{
    // we need to copy out the data because the b2Contact passed in is reused.
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    contacts.push_back(myContact);
}

void MyContactListener::EndContact(b2Contact* contact)
{
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<MyContact>::iterator pos;
    pos = std::find(contacts.begin(), contacts.end(), myContact);
    if (pos != contacts.end()) 
    {
        contacts.erase(pos);
    }
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    
}

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    
}