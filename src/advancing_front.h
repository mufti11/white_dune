/*
 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
 * http://code.google.com/p/poly2tri/
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * * Neither the name of Poly2Tri nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ADVANCED_FRONT_H
#define ADVANCED_FRONT_H

#include "shapes.h"

namespace p2t {

struct TriNode;

// Advancing front node
struct TriNode {
  Point* point;
  Triangle* triangle;

  TriNode* next;
  TriNode* prev;

  double value;

  TriNode(Point& p) : point(&p), triangle(NULL), next(NULL), prev(NULL), value(p.x)
  {
  }

  TriNode(Point& p, Triangle& t) : point(&p), triangle(&t), next(NULL), prev(NULL), value(p.x)
  {
  }

};

// Advancing front
class AdvancingFront {
public:

AdvancingFront(TriNode& head, TriNode& tail);
// Destructor
~AdvancingFront();

TriNode* head();
void set_head(TriNode* node);
TriNode* tail();
void set_tail(TriNode* node);
TriNode* search();
void set_search(TriNode* node);

/// Locate insertion point along advancing front
TriNode* LocateNode(double x);

TriNode* LocatePoint(const Point* point);

private:

TriNode* head_, *tail_, *search_node_;

TriNode* FindSearchNode(double x);
};

inline TriNode* AdvancingFront::head()
{
  return head_;
}
inline void AdvancingFront::set_head(TriNode* node)
{
  head_ = node;
}

inline TriNode* AdvancingFront::tail()
{
  return tail_;
}
inline void AdvancingFront::set_tail(TriNode* node)
{
  tail_ = node;
}

inline TriNode* AdvancingFront::search()
{
  return search_node_;
}

inline void AdvancingFront::set_search(TriNode* node)
{
  search_node_ = node;
}

}

#endif
