/*
 * List.h
 *
 * Copyright (C) 1999 Stephen F. White
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file "COPYING" for details); if 
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
 * Cambridge, MA 02139, USA.
 */

#pragma once

#ifndef NULL
#define NULL 0L
#endif

template<class T> class List {
public:
    class Iterator {
        friend class List<T>;
    public:
                        Iterator(const T &t) : m_value(t)
                           { m_prev = m_next = (Iterator *)NULL; }

        Iterator       *prev()          { return m_prev; }
        const Iterator *prev() const    { return m_prev; }
        Iterator       *next()          { return m_next; }
        const Iterator *next() const    { return m_next; }

        const T        &item() const    { return m_value; }
        T               item()          { return m_value; }
        void            setItem(const T &t)
                                        { m_value = t; }

    private:
        Iterator               *m_prev;
        Iterator               *m_next;
        T                       m_value;
    };

        void            Init(void)
                           {
                           m_head = m_tail = (Iterator *)NULL; 
                           m_length = 0;
                           }

                        List()
                           {
                           m_head = m_tail = (Iterator *)NULL; 
                           m_length = 0;
                           }

                        List(const List<T> &l)
                           {
                           m_head = m_tail = (Iterator *)NULL; 
                           m_length = 0;
                           for (Iterator *i = l.first(); i != NULL; 
                                i = i->next())
                               append(i->item());
                           }

                       ~List()
                           { 
                           removeAll();
                           }
    
        void            append(const T &t)
                           {
                           Iterator *i = new Iterator(t);
                           i->m_prev = m_tail;
                           if (m_tail)
                               m_tail->m_next = i;
                           else
                               m_head = i;
                           m_tail = i;
                           m_length++;
                           }

        void            insert(const T &t)
                           {
                           Iterator *i = new Iterator(t);
                           i->m_next = m_head;
                           if (m_head)
                               m_head->m_prev = i;
                           else
                               m_tail = i;
                           m_head = i;
                           m_length++;
                           }

        void            remove(Iterator *i)
                           {
                           if (m_length < 1)
                               return;
                           if (i->m_prev)
                               i->m_prev->m_next = i->m_next;
                           else
                               m_head = i->m_next;
                           if (i->m_next)
                               i->m_next->m_prev = i->m_prev;
                           else
                               m_tail = i->m_prev;
                           delete i;
                           m_length--;
                           }

        Iterator       *find(const T &t) const
                           {
                           for (Iterator *i = m_head; i != NULL; 
                                i = i->next())
                               if (i->item() == t) 
                                   return i;
                           return NULL;
                           }

        bool            findIterator(Iterator* iter) const
                           {
                           for (Iterator *i = m_head; i != NULL; 
                                i = i->next())
                               if (i == iter) 
                                   return true;
                           return false;
                           }

        int             findIteratorIndex(Iterator* iter) const
                           {
                           int ret = 0;
                           for (Iterator *i = m_head; i != NULL; i = i->next())
                               if (i == iter) 
                                   return ret;
                               else
                                   ret++;
                           return -1;
                           }

        Iterator       *get(int index) const
                           {
                           Iterator *i;
                           for (i = m_head; i != NULL && index > 0; 
                                i = i->next())
                               index--;
                           return i;
                           }

        void            removeAll()
                           {
                           Iterator *i, *j;
                           if (m_head != NULL)
                               for (i = m_head; i != NULL; i = j) {
                                   j = i->next();
                                   delete i;
                               }
                           Init();
                           }

        void            appendList(List<T> *list)
                           {
                           for (Iterator *i = list->first(); i != NULL;
                                i = i->next())
                               append(i->item());
                           }

        void            insertList(List<T> *list)
                           {
                           for (Iterator *i = list->last(); i != NULL;
                                i = i->prev())
                               insert(i->item());
                           }

        Iterator       *first() const 
                           { 
                           if (m_length <= 0) 
                               return NULL;
                           return m_head; 
                           }
        Iterator       *last() const 
                           { 
                           if (m_length <= 0) 
                               return NULL;
                           return m_tail; 
                           }
        int             size() const { return m_length; }

        int             operator==(const List<T> &list)
                           {
                           Iterator *i, *j;
                           for (i = list.first(), j = m_head; 
                                i != NULL && j != NULL;
                                i = i->next(), j = j->next())
                               if (i->item() != j->item())
                                   return 0;
                           return i != NULL && j != NULL;
                           }
private:
        Iterator           *m_head;
        Iterator           *m_tail;
        int                 m_length;
};

