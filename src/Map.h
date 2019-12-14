/*
 * Map.h
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

#include "List.h"

#include "config.h"

#define MAP_DEFAULT_MAX_LOAD 2
#define MAP_DEFAULT_WIDTH 16

extern int myhash(int key);

template<class K, class D> class Map {
public:
    class Pair {
    public:
                    Pair (K key, D data) { m_key = key; m_data = data; }

        K          &getKey() { return m_key; }
        D          &getData() { return m_data; }

        const K    &getKey() const { return m_key; }
        const D    &getData() const { return m_data; }

        void       setData(D data) { m_data = data; }

    private:
        K          m_key;
        D          m_data;
    };

    typedef List<Pair *> Chain;

public:
                        Map(int maxLoad = MAP_DEFAULT_MAX_LOAD) { 
                            m_maxLoad = maxLoad; 
                            m_chains = NULL;
                            m_width = m_size = 0; 
                            resize(MAP_DEFAULT_WIDTH); 
                        }
                       ~Map() {
                            TYPENAME Chain::Iterator *j;
                            for (int i = 0; i < m_width; i++) {
                                for (j = m_chains[i].first(); j != NULL; 
                                     j = j->next()) {
                                    delete j->item();
                                }
                            }
                            delete[] m_chains;
                        }

public:
    D                  &operator[](K key) {
                            static Pair *p;
                            TYPENAME Chain::Iterator *i;
                            if (m_width != 0) {
                                Chain &chain = m_chains[myhash(key) % m_width];
                                for (i = chain.first(); i != NULL; 
                                     i = i->next()) {
                                     p = i->item();
                                     if (p->getKey() == key) {
                                         return p->getData();
                                     }
                                }
                            }
                            p = add(key, 0);
                            return p->getData();
                        }
    Pair               *add(K key, D data) {
                            Pair *p = new Pair(key, data);
                            if (m_width != 0) {
                                Chain &chain = m_chains[myhash(key) % m_width];
                                chain.append(p);
                                m_size++;
                                if ( m_size > m_width * m_maxLoad )
                                    resize( m_width * 2 );
                            }
                            return p;
                        }
    void                replace(K key, D data) {
                            static Pair *p;
                            TYPENAME Chain::Iterator *i;
                            Chain &chain = m_chains[myhash(key) % m_width];
                            for (i = chain.first(); i != NULL; i = i->next()) {
                                p = i->item();
                                if (p)
                                    if (p->getKey() == key) {
                                        p->setData(data);
                                        break;
                                    }
                            }
                        }
    void                remove(K key) {
                            static Pair *p;
                            TYPENAME Chain::Iterator *i;
                            Chain &chain = m_chains[myhash(key) % m_width];
                            for (i = chain.first(); i != NULL; i = i->next()) {
                                p = i->item();
                                if (p)
                                    if (p->getKey() == key) {
                                        chain.remove(i);
                                        m_size--;
                                        break;
                                    }
                            }
                        }
    bool                hasKey(K key) {
                            static Pair *p;
                            TYPENAME Chain::Iterator *i;
                            Chain &chain = m_chains[myhash(key) % m_width];
                            for (i = chain.first(); i != NULL; i = i->next()) {
                                p = i->item();
                                if (p->getKey() == key) {
                                    return true;
                                }
                            }
                            return false;
                        }
    void                removeAll() {
                            TYPENAME Chain::Iterator *j;
                            for (int i = 0; i < m_width; i++) {
                                for (j = m_chains[i].first(); j != NULL; 
                                     j = j->next()) {
                                    delete j->item();
                                }
                            }
                            delete[] m_chains;
                            m_chains = NULL;
                            m_width = m_size = 0; 
                            resize(MAP_DEFAULT_WIDTH); 
                        }
    int                 width() { return m_width; }
    const Chain        &chain(int num) const { return m_chains[num]; }
    int                 size(void) { return m_size; }

private:
    void                resize(int width);

private:
    int                 m_size;
    int                 m_width;
    int                 m_maxLoad;
    Chain              *m_chains;
};

template<class K, class D>
inline void             Map<K, D>::resize(int width) {
                            Chain *newChains = new Chain[width];
                            TYPENAME Chain::Iterator *j;
                            for (int i = 0; i < m_width; i++) {
                                for (j = m_chains[i].first(); j != NULL;
                                     j = j->next()) {
                                    Pair *p = j->item();
                                    if (width != 0)
                                        newChains[myhash(p->getKey()) % width]
                                                 .append(p);
                                }
                            }
                            delete[] m_chains;
                            m_chains = newChains;
                            m_width = width;
                        }
