//
//  StringHelp.h
//  BoilThisPlate
//
//  Created by Nicholas Alcus on 9/6/16.
//  Copyright © 2016 Nicholas Alcus. All rights reserved.
//

#ifndef StringHelp_h
#define StringHelp_h

// from sfml book

#include <sstream>

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value);

#include "StringHelp.inl"

#endif /* StringHelp_h */
