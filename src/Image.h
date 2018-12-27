/*
 * Image.h
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

#ifndef _IMAGE_H
#define _IMAGE_H

class Image {
public:
                        Image();
                       ~Image();

    typedef enum { GIF, JPEG, PNG } Type;

    bool                Open(const char *filename);
    void                Read(unsigned char *data);

    int                 GetWidth() const { return m_width; }
    int                 GetHeight() const { return m_height; }
    int                 GetComponents() const { return m_components; }

    int                 test_file_extension(char *extension);
    char*               file_extension(const char *filename);


private:
    Type                m_type;
    int                 m_width;
    int                 m_height;
    int                 m_components;
};

#endif // _IMAGE_H
