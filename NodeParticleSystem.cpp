/*
 * NodeParticleSystem.cpp
 *
 * Copyright (C) 2009, 2019 J. "MUFTI" Scheurich
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

// Special thanks to Braden McDaniel

#include <stdio.h>
#include "stdafx.h"
#include "swt.h"

#include "NodeParticleSystem.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFNode.h"
#include "SFBool.h"
#include "SFNode.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFInt32.h"
#include "SFFloat.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "SFNode.h"
#include "MFFloat.h"
#include "SFNode.h"
#include "SFString.h"
#include "MFNode.h"
#include "SFNode.h"
#include "MFFloat.h"
#include "DuneApp.h"
#include "Scene.h"
#include "NodeForcePhysicsModel.h"
#include "NodeWindPhysicsModel.h"
#include "NodeConeEmitter.h"
#include "NodeExplosionEmitter.h"
#include "NodePointEmitter.h"

#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodePointSet.h"

#ifdef _WIN32
# define random(x) rand(x)
#endif

ProtoParticleSystem::ProtoParticleSystem(Scene *scene)
  : Proto(scene, "ParticleSystem")
{
    appearance.set(
        addExposedField(SFNODE, "appearance", new SFNode(), VRML_APPEARANCE));
    createParticles.set(
        addExposedField(SFBOOL, "createParticles", new SFBool(true)));
    geometry.set(
        addExposedField(SFNODE, "geometry", new SFNode(), GEOMETRY_NODE));
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    lifetimeVariation.set(
        addExposedField(SFFLOAT, "lifetimeVariation", new SFFloat(0.25),
                        new SFFloat(0), new SFFloat(1)));
    maxParticles.set(
        addExposedField(SFINT32, "maxParticles", new SFInt32(200),
                        new SFInt32(0)));
    particleLifetime.set(
        addExposedField(SFFLOAT, "particleLifetime", new SFFloat(5), 
                        new SFFloat(0)));
    particleSize.set(
        addExposedField(SFVEC2F, "particleSize", new SFVec2f(0.02, 0.02)));
    bboxCenter.set(
        addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
        addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1)));
    colorRamp.set(
        addField(SFNODE, "colorRamp", new SFNode(), COLOR_NODE));
    colorKey.set(
        addField(MFFLOAT, "colorKey", new MFFloat()));
    emitter.set(
        addField(SFNODE, "emitter", new SFNode(), PARTICLE_EMITTER_NODE));
    geometryType.set(
        addField(SFSTRING, "geometryType", new SFString("QUAD")));
    physics.set(
        addField(MFNODE, "physics", new MFNode(), PARTICLE_PHYSICS_MODEL_NODE));
    texCoordRamp.set(
        addField(SFNODE, "texCoordRamp", new SFNode(), VRML_TEXTURE_COORDINATE));
    texCoordKey.set(
        addField(MFFLOAT, "texCoordKey", new MFFloat()));
}

Node *
ProtoParticleSystem::create(Scene *scene)
{ 
    return new NodeParticleSystem(scene, this); 
}

NodeParticleSystem::NodeParticleSystem(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_particlesDirty = true;
    m_force[0] = 0;
    m_force[1] = 0;
    m_force[2] = 0;
    m_internTime = 0;
    m_mass = 0;
    m_particleNode = NULL;
}

int
NodeParticleSystem::getComponentLevel(void) const 
{ 
     const char *type = geometryType()->getValue();
     if ((geometry()->getValue() != NULL) ||
         ((type != NULL) && (strcmp(type, "GEOMETRY") != 0)))
          return 4;
     return 3; 
}

void    
NodeParticleSystem::draw(int pass)
{
    init();

    glMatrixMode(GL_MODELVIEW);
    Quaternion quat = m_scene->getCamera()->getOrientation();
    SFRotation rot(quat);
    const float *frotation = rot.getValue();

    if (!(enabled()->getValue()))
        return;
    Node *particleNode = NULL;
    if (m_geometryType == PARTICLES_GEOMETRY)
        particleNode = geometry()->getValue();
    else
        particleNode = m_particleNode;
    if (!m_scene->isRunning()) {
        if (appearance()->getValue())
            appearance()->getValue()->bind();
        glPushMatrix();
        if (m_geometryType == PARTICLES_SPRITE)
            glRotatef(RAD2DEG(frotation[3]),
                      frotation[0], frotation[1], frotation[2]);
        if (particleNode)
            particleNode->draw();
        glPopMatrix();
        if (appearance()->getValue())
            appearance()->getValue()->unbind();
        return;
    }
    for (int j = 0; j < 3; j ++)
         m_force[j] += 0;
    
    for (int i = 0; i < physics()->getSize(); i++)
        if (physics()->getValue(i)) {
            Node *phys = physics()->getValue(i);
            if (phys->getType() == X3D_FORCE_PHYSICS_MODEL) {
                NodeForcePhysicsModel *model = (NodeForcePhysicsModel *)phys;
                const float *forceForce = model->force()->getValue();
                for (int j = 0; j < 3; j ++)
                    m_force[j] += forceForce[j];
            } else if (phys->getType() == X3D_WIND_PHYSICS_MODEL) {
                NodeWindPhysicsModel *model = (NodeWindPhysicsModel *)phys;
                Vec3f forceWind = model->getForce();
                m_force[0] += forceWind.x;
                m_force[1] += forceWind.y;
                m_force[2] += forceWind.z;
            } 
        }
    int numberParticles = maxParticles()->getValue();

    double t = swGetCurrentTime();
    double delta = 0;
    if (m_internTime == 0)
        delta = 0;
    else
       delta = t - m_internTime;
    for (int i = 0; i < numberParticles; i++) {
        if ((t - m_startTime[i]) > m_lifeTime[i])
            startParticle(i);
        else  {        
            m_internVector[i].x += delta * m_force[0];
            m_internVector[i].y += delta * m_force[1];
            m_internVector[i].z += delta * m_force[2];
        }
        float mass = m_mass;
        if (mass == 0.0f)
            mass = 1.0f;
        m_internPosition[i].x += mass * m_internVector[i].x * delta;
        m_internPosition[i].y += mass * m_internVector[i].y * delta;
        m_internPosition[i].z += mass * m_internVector[i].z * delta;

        glPushMatrix();
        glTranslatef(m_internPosition[i].x,
                     m_internPosition[i].y,
                     m_internPosition[i].z);
        if (m_geometryType == PARTICLES_SPRITE)
            glRotatef(RAD2DEG(frotation[3]),
                      frotation[0], frotation[1], frotation[2]);
        if (appearance()->getValue())
            appearance()->getValue()->bind();
        if (particleNode)
            particleNode->draw();
        if (appearance()->getValue())
            appearance()->getValue()->unbind();
        glPopMatrix();
    }
    m_internTime = swGetCurrentTime();
}

void NodeParticleSystem::startParticle(int i)
{
    float speed = 1;
    m_internPosition[i].x = 0;
    m_internPosition[i].y = 0; 
    m_internPosition[i].z = 0;
    float alpha = (random() / (float)RAND_MAX) * 2.0 * M_PI;
    float maxAngle = 2.0 * M_PI;
    if (emitter()->getValue())
        if (emitter()->getValue()->getType() == X3D_CONE_EMITTER) {
            NodeConeEmitter *emit = (NodeConeEmitter *) 
                                    emitter()->getValue();
            maxAngle = emit->angle()->getValue();
        }
    float angle = (random() / (float)RAND_MAX) * maxAngle; 

    if (emitter()->getValue())
        if (emitter()->getValue()->getType() == X3D_CONE_EMITTER) {
            NodeConeEmitter *emit = (NodeConeEmitter *)
                                    emitter()->getValue();
            m_internPosition[i].x = emit->position()->getValue()[0];
            m_internPosition[i].y = emit->position()->getValue()[1]; 
            m_internPosition[i].z = emit->position()->getValue()[2];
            speed = (random() / (float)RAND_MAX) * 
                    (emit->variation()->getValue() / 2.0f + 1) *
                    emit->speed()->getValue();
/*
            if ((emit->direction()->getValue()[0] != 0) &&
                (emit->direction()->getValue()[1] != 0) &&
                (emit->direction()->getValue()[0] != 0)) {
                m_internVector[i].x = speed * emit->direction()->getValue()[0];
                m_internVector[i].y = speed * emit->direction()->getValue()[1];
                m_internVector[i].z = speed * emit->direction()->getValue()[2];
            }
*/
            m_mass = emit->mass()->getValue();
        } else if (emitter()->getValue()->getType() == X3D_POINT_EMITTER) {
            NodePointEmitter *emit = (NodePointEmitter *) 
                                     emitter()->getValue();
            m_internPosition[i].x = emit->position()->getValue()[0];
            m_internPosition[i].y = emit->position()->getValue()[1]; 
            m_internPosition[i].z = emit->position()->getValue()[2];
            speed = (random() / (float)RAND_MAX) * 
                    (emit->variation()->getValue() / 2.0f + 1) *
                    emit->speed()->getValue();
            if ((emit->direction()->getValue()[0] != 0) &&
                (emit->direction()->getValue()[1] != 0) &&
                (emit->direction()->getValue()[0] != 0)) {
                m_internVector[i].x = speed * 
                                      emit->direction()->getValue()[0];
                m_internVector[i].y = speed * 
                                      emit->direction()->getValue()[1];
                m_internVector[i].z = speed * 
                                      emit->direction()->getValue()[2];
            }
            m_mass = emit->mass()->getValue();
        } else if (emitter()->getValue()->getType() == 
                   X3D_EXPLOSION_EMITTER) {
            NodeExplosionEmitter *emit = (NodeExplosionEmitter *) 
                                         emitter()->getValue();
            m_internPosition[i].x = emit->position()->getValue()[0];
            m_internPosition[i].y = emit->position()->getValue()[1]; 
            m_internPosition[i].z = emit->position()->getValue()[2];
            speed = (random() / (float)RAND_MAX) * 
                    (emit->variation()->getValue() / 2.0f + 1) *
                    emit->speed()->getValue();
            m_mass = emit->mass()->getValue();
        }

    m_internVector[i].x = speed * m_force[0] * sin(angle) * cos(alpha);
    m_internVector[i].y = speed * m_force[1] * cos(angle);
    m_internVector[i].z = speed * m_force[2] * sin(angle) * sin(alpha);

    m_lifeTime[i] = (random() / (float)RAND_MAX) * 
                    (lifetimeVariation()->getValue() / 2.0f + 1) *
                    particleLifetime()->getValue(); 
    m_startTime[i] = swGetCurrentTime();
}

void 
NodeParticleSystem::init(void)
{
    if (m_particlesDirty) {
        if (strcmp((char *)geometryType()->getValue(), "SPRITE") == 0)
            m_geometryType = PARTICLES_SPRITE;
        else
            m_geometryType = PARTICLES_QUAD;
        if (strcmp((char *)geometryType()->getValue(), "GEOMETRY") == 0)
            m_geometryType = PARTICLES_GEOMETRY;
        else if (strcmp((char *)geometryType()->getValue(), "POINT")
                 == 0) {
            m_geometryType = PARTICLES_POINT;
            NodePointSet *set = (NodePointSet *)
                                m_scene->createNode("PointSet");
            NodeCoordinate *coords = (NodeCoordinate *)
                                     m_scene->createNode("Coordinate");
            MFVec3f *points = new MFVec3f();
            Vec3f vec;
            points->setVec(0, vec);
            coords->point(points);
            set->coord(new SFNode(coords));
            m_particleNode = set;
        } else if (strcmp((char *)geometryType()->getValue(), "LINE")
                 == 0) {
            NodeIndexedLineSet *lineSet = (NodeIndexedLineSet *)
                m_scene->createNode("IndexedLineSet");
            NodeCoordinate *coords = (NodeCoordinate *)
                                     m_scene->createNode("Coordinate");
            MFVec3f *points = new MFVec3f();
            const float* size = particleSize()->getValue();
            Vec3f vec1(0, -size[1] / 2.0f, 0);
            points->setVec(0, vec1);
            Vec3f vec2(0,  size[1] / 2.0f, 0);
            points->setVec(1, vec2);
            coords->point(points);
            MFInt32 *ci = new MFInt32();
            lineSet->coord(new SFNode(coords));
            ci->appendSFValue(0);
            ci->appendSFValue(1);
            lineSet->coordIndex(ci);
            m_particleNode = lineSet;
        } else {
            NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *)
                m_scene->createNode("IndexedFaceSet");
            NodeCoordinate *coords = (NodeCoordinate *)
                                     m_scene->createNode("Coordinate");
            MFVec3f *points = new MFVec3f();
            const float* size = particleSize()->getValue();
            Vec3f vec1(-size[0] / 2.0f, -size[1] / 2.0f, 0);
            points->setVec(0, vec1);
            Vec3f vec2( size[0] / 2.0f, -size[1] / 2.0f, 0);
            points->setVec(1, vec2);
            Vec3f vec3( size[0] / 2.0f,  size[1] / 2.0f, 0);
            points->setVec(2, vec3);
            Vec3f vec4(-size[0] / 2.0f,  size[1] / 2.0f, 0);
            points->setVec(3, vec4);
            coords->point(points);
            MFInt32 *ci = new MFInt32();
            faceSet->coord(new SFNode(coords));
            ci->appendSFValue(0);
            ci->appendSFValue(1);
            ci->appendSFValue(2);
            ci->appendSFValue(3);
            faceSet->coordIndex(ci);
            m_particleNode = faceSet;
        }
        m_internPosition.resize(0);
        m_internVector.resize(0);
        m_lifeTime.resize(0);
        m_startTime.resize(0);
        int numberParticles = maxParticles()->getValue();
        for (int i = 0; i < numberParticles; i++)
            startParticle(i);
        m_particlesDirty = false;
    }
}
