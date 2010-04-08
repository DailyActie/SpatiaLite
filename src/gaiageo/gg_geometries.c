/*

 gg_geometries.c -- Gaia geometric objects
  
 version 2.4, 2009 September 17

 Author: Sandro Furieri a.furieri@lqt.it

 ------------------------------------------------------------------------------
 
 Version: MPL 1.1/GPL 2.0/LGPL 2.1
 
 The contents of this file are subject to the Mozilla Public License Version
 1.1 (the "License"); you may not use this file except in compliance with
 the License. You may obtain a copy of the License at
 http://www.mozilla.org/MPL/
 
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

The Original Code is the SpatiaLite library

The Initial Developer of the Original Code is Alessandro Furieri
 
Portions created by the Initial Developer are Copyright (C) 2008
the Initial Developer. All Rights Reserved.

Contributor(s):

Alternatively, the contents of this file may be used under the terms of
either the GNU General Public License Version 2 or later (the "GPL"), or
the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
in which case the provisions of the GPL or the LGPL are applicable instead
of those above. If you wish to allow use of your version of this file only
under the terms of either the GPL or the LGPL, and not to allow others to
use your version of this file under the terms of the MPL, indicate your
decision by deleting the provisions above and replace them with the notice
and other provisions required by the GPL or the LGPL. If you do not delete
the provisions above, a recipient may use your version of this file under
the terms of any one of the MPL, the GPL or the LGPL.
 
*/

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <float.h>

#include <spatialite/sqlite3ext.h>
#include <spatialite/gaiageo.h>

GAIAGEO_DECLARE gaiaPointPtr
gaiaAllocPoint (double x, double y)
{
/* POINT object constructor */
    gaiaPointPtr p = malloc (sizeof (gaiaPoint));
    p->X = x;
    p->Y = y;
    p->Z = 0.0;
    p->M = 0.0;
    p->DimensionModel = GAIA_XY;
    p->Next = NULL;
    p->Prev = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaPointPtr
gaiaAllocPointXYZ (double x, double y, double z)
{
/* POINT object constructor */
    gaiaPointPtr p = malloc (sizeof (gaiaPoint));
    p->X = x;
    p->Y = y;
    p->Z = z;
    p->M = 0.0;
    p->DimensionModel = GAIA_XY_Z;
    p->Next = NULL;
    p->Prev = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaPointPtr
gaiaAllocPointXYM (double x, double y, double m)
{
/* POINT object constructor */
    gaiaPointPtr p = malloc (sizeof (gaiaPoint));
    p->X = x;
    p->Y = y;
    p->Z = 0.0;
    p->M = m;
    p->DimensionModel = GAIA_XY_M;
    p->Next = NULL;
    p->Prev = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaPointPtr
gaiaAllocPointXYZM (double x, double y, double z, double m)
{
/* POINT object constructor */
    gaiaPointPtr p = malloc (sizeof (gaiaPoint));
    p->X = x;
    p->Y = y;
    p->Z = z;
    p->M = m;
    p->DimensionModel = GAIA_XY_Z_M;
    p->Next = NULL;
    p->Prev = NULL;
    return p;
}

GAIAGEO_DECLARE void
gaiaFreePoint (gaiaPointPtr ptr)
{
/* POINT object destructor */
    if (ptr != NULL)
	free (ptr);
}

GAIAGEO_DECLARE gaiaLinestringPtr
gaiaAllocLinestring (int vert)
{
/* LINESTRING object constructor */
    gaiaLinestringPtr p = malloc (sizeof (gaiaLinestring));
    p->Coords = malloc (sizeof (double) * (vert * 2));
    p->Points = vert;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaLinestringPtr
gaiaAllocLinestringXYZ (int vert)
{
/* LINESTRING object constructor */
    gaiaLinestringPtr p = malloc (sizeof (gaiaLinestring));
    p->Coords = malloc (sizeof (double) * (vert * 3));
    p->Points = vert;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_Z;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaLinestringPtr
gaiaAllocLinestringXYM (int vert)
{
/* LINESTRING object constructor */
    gaiaLinestringPtr p = malloc (sizeof (gaiaLinestring));
    p->Coords = malloc (sizeof (double) * (vert * 3));
    p->Points = vert;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_M;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaLinestringPtr
gaiaAllocLinestringXYZM (int vert)
{
/* LINESTRING object constructor */
    gaiaLinestringPtr p = malloc (sizeof (gaiaLinestring));
    p->Coords = malloc (sizeof (double) * (vert * 4));
    p->Points = vert;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_Z_M;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE void
gaiaFreeLinestring (gaiaLinestringPtr ptr)
{
/* LINESTRING object desctructror */
    if (ptr)
      {
	  if (ptr->Coords)
	      free (ptr->Coords);
	  free (ptr);
      }
}

GAIAGEO_DECLARE int
gaiaLineGetPoint (gaiaLinestringPtr ln, int v, double *x, double *y, double *z,
		  double *m)
{
/* SAFE - getting coords for a vertex in LINESTRING */
    double vx;
    double vy;
    double vz;
    double vm;
    *x = 0.0;
    *y = 0.0;
    *z = 0.0;
    *m = 0.0;
    if (!ln)
	return 0;
    if (v < 0 || v >= ln->Points)
	return 0;
    switch (ln->DimensionModel)
      {
      case GAIA_XY:
	  gaiaGetPoint (ln->Coords, v, &vx, &vy);
	  *x = vx;
	  *y = vy;
	  break;
      case GAIA_XY_Z:
	  gaiaGetPointXYZ (ln->Coords, v, &vx, &vy, &vz);
	  *x = vx;
	  *y = vy;
	  *z = vz;
	  break;
      case GAIA_XY_M:
	  gaiaGetPointXYM (ln->Coords, v, &vx, &vy, &vm);
	  *x = vx;
	  *y = vy;
	  *m = vm;
	  break;
      case GAIA_XY_Z_M:
	  gaiaGetPointXYZM (ln->Coords, v, &vx, &vy, &vz, &vm);
	  *x = vx;
	  *y = vy;
	  *z = vz;
	  *m = vm;
	  break;
      default:
	  return 0;
      };
    return 1;
}

GAIAGEO_DECLARE int
gaiaLineSetPoint (gaiaLinestringPtr ln, int v, double x, double y, double z,
		  double m)
{
/* SAFE - setting coords for a vertex in RING */
    if (!ln)
	return 0;
    if (v < 0 || v >= ln->Points)
	return 0;
    switch (ln->DimensionModel)
      {
      case GAIA_XY:
	  gaiaSetPoint (ln->Coords, v, x, y);
	  break;
      case GAIA_XY_Z:
	  gaiaSetPointXYZ (ln->Coords, v, x, y, z);
	  break;
      case GAIA_XY_M:
	  gaiaSetPointXYM (ln->Coords, v, x, y, m);
	  break;
      case GAIA_XY_Z_M:
	  gaiaSetPointXYZM (ln->Coords, v, x, y, z, m);
	  break;
      default:
	  return 0;
      };
    return 1;
}

GAIAGEO_DECLARE void
gaiaCopyLinestringCoords (gaiaLinestringPtr dst, gaiaLinestringPtr src)
{
/* 
/ copying coords from one Linestring to another
/ maybe, converting from one Dimension Model to a different one
*/
    int iv;
    double x;
    double y;
    double z;
    double m;
    if (!src)
	return;
    if (!dst)
	return;
    if (src->Points != dst->Points)
	return;
    for (iv = 0; iv < dst->Points; iv++)
      {
	  z = 0.0;
	  m = 0.0;
	  if (src->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (src->Coords, iv, &x, &y, &z);
	    }
	  else if (src->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (src->Coords, iv, &x, &y, &m);
	    }
	  else if (src->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (src->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (src->Coords, iv, &x, &y);
	    }
	  if (dst->DimensionModel == GAIA_XY_Z)
	    {
		gaiaSetPointXYZ (dst->Coords, iv, x, y, z);
	    }
	  else if (dst->DimensionModel == GAIA_XY_M)
	    {
		gaiaSetPointXYM (dst->Coords, iv, x, y, m);
	    }
	  else if (dst->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaSetPointXYZM (dst->Coords, iv, x, y, z, m);
	    }
	  else
	    {
		gaiaSetPoint (dst->Coords, iv, x, y);
	    }
      }
}

GAIAGEO_DECLARE gaiaLinestringPtr
gaiaCloneLinestring (gaiaLinestringPtr line)
{
/* clones a LINESTRING */
    gaiaLinestringPtr new_line;
    if (!line)
	return NULL;
    if (line->DimensionModel == GAIA_XY_Z)
	new_line = gaiaAllocLinestringXYZ (line->Points);
    else if (line->DimensionModel == GAIA_XY_M)
	new_line = gaiaAllocLinestringXYM (line->Points);
    else if (line->DimensionModel == GAIA_XY_Z_M)
	new_line = gaiaAllocLinestringXYZM (line->Points);
    else
	new_line = gaiaAllocLinestring (line->Points);
    gaiaCopyLinestringCoords (new_line, line);
    return new_line;
}

GAIAGEO_DECLARE gaiaRingPtr
gaiaAllocRing (int vert)
{
/* ring object constructor */
    gaiaRingPtr p = malloc (sizeof (gaiaRing));
    p->Coords = malloc (sizeof (double) * (vert * 2));
    p->Points = vert;
    p->Link = NULL;
    p->Clockwise = 0;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaRingPtr
gaiaAllocRingXYZ (int vert)
{
/* ring object constructor */
    gaiaRingPtr p = malloc (sizeof (gaiaRing));
    p->Coords = malloc (sizeof (double) * (vert * 3));
    p->Points = vert;
    p->Link = NULL;
    p->Clockwise = 0;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_Z;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaRingPtr
gaiaAllocRingXYM (int vert)
{
/* ring object constructor */
    gaiaRingPtr p = malloc (sizeof (gaiaRing));
    p->Coords = malloc (sizeof (double) * (vert * 3));
    p->Points = vert;
    p->Link = NULL;
    p->Clockwise = 0;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_M;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaRingPtr
gaiaAllocRingXYZM (int vert)
{
/* ring object constructor */
    gaiaRingPtr p = malloc (sizeof (gaiaRing));
    p->Coords = malloc (sizeof (double) * (vert * 4));
    p->Points = vert;
    p->Link = NULL;
    p->Clockwise = 0;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_Z_M;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE void
gaiaFreeRing (gaiaRingPtr ptr)
{
/* ring object destructor */
    if (ptr)
      {
	  if (ptr->Coords)
	      free (ptr->Coords);
	  free (ptr);
      }
}

GAIAGEO_DECLARE int
gaiaRingGetPoint (gaiaRingPtr rng, int v, double *x, double *y, double *z,
		  double *m)
{
/* SAFE - getting coords for a vertex in RING */
    double vx;
    double vy;
    double vz;
    double vm;
    *x = 0.0;
    *y = 0.0;
    *z = 0.0;
    *m = 0.0;
    if (!rng)
	return 0;
    if (v < 0 || v >= rng->Points)
	return 0;
    switch (rng->DimensionModel)
      {
      case GAIA_XY:
	  gaiaGetPoint (rng->Coords, v, &vx, &vy);
	  *x = vx;
	  *y = vy;
	  break;
      case GAIA_XY_Z:
	  gaiaGetPointXYZ (rng->Coords, v, &vx, &vy, &vz);
	  *x = vx;
	  *y = vy;
	  *z = vz;
	  break;
      case GAIA_XY_M:
	  gaiaGetPointXYM (rng->Coords, v, &vx, &vy, &vm);
	  *x = vx;
	  *y = vy;
	  *m = vm;
	  break;
      case GAIA_XY_Z_M:
	  gaiaGetPointXYZM (rng->Coords, v, &vx, &vy, &vz, &vm);
	  *x = vx;
	  *y = vy;
	  *z = vz;
	  *m = vm;
	  break;
      default:
	  return 0;
      };
    return 1;
}

GAIAGEO_DECLARE int
gaiaRingSetPoint (gaiaRingPtr rng, int v, double x, double y, double z,
		  double m)
{
/* SAFE - getting coords for a vertex in RING */
    if (!rng)
	return 0;
    if (v < 0 || v >= rng->Points)
	return 0;
    switch (rng->DimensionModel)
      {
      case GAIA_XY:
	  gaiaSetPoint (rng->Coords, v, x, y);
	  break;
      case GAIA_XY_Z:
	  gaiaSetPointXYZ (rng->Coords, v, x, y, z);
	  break;
      case GAIA_XY_M:
	  gaiaSetPointXYM (rng->Coords, v, x, y, m);
	  break;
      case GAIA_XY_Z_M:
	  gaiaSetPointXYZM (rng->Coords, v, x, y, z, m);
	  break;
      default:
	  return 0;
      };
    return 1;
}

GAIAGEO_DECLARE void
gaiaCopyRingCoords (gaiaRingPtr dst, gaiaRingPtr src)
{
/* 
/ copying coords from one Ring to another
/ maybe, converting from one Dimension Model to a different one
*/
    int iv;
    double x;
    double y;
    double z;
    double m;
    if (!src)
	return;
    if (!dst)
	return;
    if (src->Points != dst->Points)
	return;
    for (iv = 0; iv < dst->Points; iv++)
      {
	  z = 0.0;
	  m = 0.0;
	  if (src->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (src->Coords, iv, &x, &y, &z);
	    }
	  else if (src->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (src->Coords, iv, &x, &y, &m);
	    }
	  else if (src->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (src->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (src->Coords, iv, &x, &y);
	    }
	  if (dst->DimensionModel == GAIA_XY_Z)
	    {
		gaiaSetPointXYZ (dst->Coords, iv, x, y, z);
	    }
	  else if (dst->DimensionModel == GAIA_XY_M)
	    {
		gaiaSetPointXYM (dst->Coords, iv, x, y, m);
	    }
	  else if (dst->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaSetPointXYZM (dst->Coords, iv, x, y, z, m);
	    }
	  else
	    {
		gaiaSetPoint (dst->Coords, iv, x, y);
	    }
      }
}

GAIAGEO_DECLARE gaiaRingPtr
gaiaCloneRing (gaiaRingPtr ring)
{
/* clones a RING */
    gaiaRingPtr new_ring;
    if (!ring)
	return NULL;
    if (ring->DimensionModel == GAIA_XY_Z)
	new_ring = gaiaAllocRingXYZ (ring->Points);
    else if (ring->DimensionModel == GAIA_XY_M)
	new_ring = gaiaAllocRing (ring->Points);
    else if (ring->DimensionModel == GAIA_XY_Z_M)
	new_ring = gaiaAllocRingXYZM (ring->Points);
    else
	new_ring = gaiaAllocRing (ring->Points);
    gaiaCopyRingCoords (new_ring, ring);
    return new_ring;
}

GAIAGEO_DECLARE gaiaPolygonPtr
gaiaClonePolygon (gaiaPolygonPtr polyg)
{
/* clones a POLYGON */
    int ib;
    gaiaPolygonPtr new_polyg;
    gaiaRingPtr i_ring;
    gaiaRingPtr o_ring;
    if (!polyg)
	return NULL;
    i_ring = polyg->Exterior;
    if (polyg->DimensionModel == GAIA_XY_Z)
	new_polyg = gaiaAllocPolygonXYZ (i_ring->Points, polyg->NumInteriors);
    else if (polyg->DimensionModel == GAIA_XY_M)
	new_polyg = gaiaAllocPolygonXYM (i_ring->Points, polyg->NumInteriors);
    else if (polyg->DimensionModel == GAIA_XY_Z_M)
	new_polyg = gaiaAllocPolygonXYZM (i_ring->Points, polyg->NumInteriors);
    else
	new_polyg = gaiaAllocPolygon (i_ring->Points, polyg->NumInteriors);
    o_ring = new_polyg->Exterior;
/* copying points for the EXTERIOR RING */
    gaiaCopyRingCoords (o_ring, i_ring);
    for (ib = 0; ib < new_polyg->NumInteriors; ib++)
      {
	  /* copying each INTERIOR RING [if any] */
	  i_ring = polyg->Interiors + ib;
	  o_ring = gaiaAddInteriorRing (new_polyg, ib, i_ring->Points);
	  gaiaCopyRingCoords (o_ring, i_ring);
      }
    return new_polyg;
}

GAIAGEO_DECLARE gaiaPolygonPtr
gaiaAllocPolygon (int vert, int excl)
{
/* POLYGON object constructor */
    gaiaPolygonPtr p;
    gaiaRingPtr pP;
    int ind;
    p = malloc (sizeof (gaiaPolygon));
    p->Exterior = gaiaAllocRing (vert);
    p->NumInteriors = excl;
    p->NextInterior = 0;
    p->Next = NULL;
    if (excl == 0)
	p->Interiors = NULL;
    else
	p->Interiors = malloc (sizeof (gaiaRing) * excl);
    for (ind = 0; ind < p->NumInteriors; ind++)
      {
	  pP = p->Interiors + ind;
	  pP->Points = 0;
	  pP->Coords = NULL;
	  pP->Next = NULL;
	  pP->Link = 0;
      }
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY;
    return p;
}

GAIAGEO_DECLARE gaiaPolygonPtr
gaiaAllocPolygonXYZ (int vert, int excl)
{
/* POLYGON object constructor */
    gaiaPolygonPtr p;
    gaiaRingPtr pP;
    int ind;
    p = malloc (sizeof (gaiaPolygon));
    p->Exterior = gaiaAllocRingXYZ (vert);
    p->NumInteriors = excl;
    p->NextInterior = 0;
    p->Next = NULL;
    if (excl == 0)
	p->Interiors = NULL;
    else
	p->Interiors = malloc (sizeof (gaiaRing) * excl);
    for (ind = 0; ind < p->NumInteriors; ind++)
      {
	  pP = p->Interiors + ind;
	  pP->Points = 0;
	  pP->Coords = NULL;
	  pP->Next = NULL;
	  pP->Link = 0;
      }
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_Z;
    return p;
}

GAIAGEO_DECLARE gaiaPolygonPtr
gaiaAllocPolygonXYM (int vert, int excl)
{
/* POLYGON object constructor */
    gaiaPolygonPtr p;
    gaiaRingPtr pP;
    int ind;
    p = malloc (sizeof (gaiaPolygon));
    p->Exterior = gaiaAllocRingXYM (vert);
    p->NumInteriors = excl;
    p->NextInterior = 0;
    p->Next = NULL;
    if (excl == 0)
	p->Interiors = NULL;
    else
	p->Interiors = malloc (sizeof (gaiaRing) * excl);
    for (ind = 0; ind < p->NumInteriors; ind++)
      {
	  pP = p->Interiors + ind;
	  pP->Points = 0;
	  pP->Coords = NULL;
	  pP->Next = NULL;
	  pP->Link = 0;
      }
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_M;
    return p;
}

GAIAGEO_DECLARE gaiaPolygonPtr
gaiaAllocPolygonXYZM (int vert, int excl)
{
/* POLYGON object constructor */
    gaiaPolygonPtr p;
    gaiaRingPtr pP;
    int ind;
    p = malloc (sizeof (gaiaPolygon));
    p->Exterior = gaiaAllocRingXYZM (vert);
    p->NumInteriors = excl;
    p->NextInterior = 0;
    p->Next = NULL;
    if (excl == 0)
	p->Interiors = NULL;
    else
	p->Interiors = malloc (sizeof (gaiaRing) * excl);
    for (ind = 0; ind < p->NumInteriors; ind++)
      {
	  pP = p->Interiors + ind;
	  pP->Points = 0;
	  pP->Coords = NULL;
	  pP->Next = NULL;
	  pP->Link = 0;
      }
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_Z_M;
    return p;
}

GAIAGEO_DECLARE gaiaPolygonPtr
gaiaCreatePolygon (gaiaRingPtr ring)
{
/* POLYGON object constructor */
    gaiaPolygonPtr p;
    p = malloc (sizeof (gaiaPolygon));
    p->DimensionModel = ring->DimensionModel;
    if (ring->DimensionModel == GAIA_XY_Z)
	p->Exterior = gaiaAllocRingXYZ (ring->Points);
    else if (ring->DimensionModel == GAIA_XY_M)
	p->Exterior = gaiaAllocRingXYM (ring->Points);
    else if (ring->DimensionModel == GAIA_XY_Z_M)
	p->Exterior = gaiaAllocRingXYZM (ring->Points);
    else
	p->Exterior = gaiaAllocRing (ring->Points);
    p->NumInteriors = 0;
    p->NextInterior = 0;
    p->Next = NULL;
    p->Interiors = NULL;
    gaiaCopyRingCoords (p->Exterior, ring);
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    return p;
}

GAIAGEO_DECLARE void
gaiaFreePolygon (gaiaPolygonPtr p)
{
/* POLYGON object destructor */
    gaiaRingPtr pP;
    int ind;
    if (p->Exterior)
	gaiaFreeRing (p->Exterior);
    for (ind = 0; ind < p->NumInteriors; ind++)
      {
	  pP = p->Interiors + ind;
	  if (pP->Coords)
	      free (pP->Coords);
      }
    if (p->Interiors)
	free (p->Interiors);
    free (p);
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaCloneGeomColl (gaiaGeomCollPtr geom)
{
/* clones a GEOMETRYCOLLECTION */
    int ib;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaLinestringPtr new_line;
    gaiaPolygonPtr polyg;
    gaiaPolygonPtr new_polyg;
    gaiaGeomCollPtr new_geom;
    gaiaRingPtr i_ring;
    gaiaRingPtr o_ring;
    if (!geom)
	return NULL;
    if (geom->DimensionModel == GAIA_XY_Z)
	new_geom = gaiaAllocGeomCollXYZ ();
    else if (geom->DimensionModel == GAIA_XY_M)
	new_geom = gaiaAllocGeomCollXYM ();
    else if (geom->DimensionModel == GAIA_XY_Z_M)
	new_geom = gaiaAllocGeomCollXYZM ();
    else
	new_geom = gaiaAllocGeomColl ();
    new_geom->Srid = geom->Srid;
    new_geom->DeclaredType = geom->DeclaredType;
    point = geom->FirstPoint;
    while (point)
      {
	  /* copying POINTs */
	  gaiaAddPointToGeomCollXYZM (new_geom, point->X, point->Y, point->Z,
				      point->M);
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* copying LINESTRINGs */
	  new_line = gaiaAddLinestringToGeomColl (new_geom, line->Points);
	  gaiaCopyLinestringCoords (new_line, line);
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* copying POLYGONs */
	  i_ring = polyg->Exterior;
	  new_polyg =
	      gaiaAddPolygonToGeomColl (new_geom, i_ring->Points,
					polyg->NumInteriors);
	  o_ring = new_polyg->Exterior;
	  /* copying points for the EXTERIOR RING */
	  gaiaCopyRingCoords (o_ring, i_ring);
	  for (ib = 0; ib < new_polyg->NumInteriors; ib++)
	    {
		/* copying each INTERIOR RING [if any] */
		i_ring = polyg->Interiors + ib;
		o_ring = gaiaAddInteriorRing (new_polyg, ib, i_ring->Points);
		gaiaCopyRingCoords (o_ring, i_ring);
	    }
	  polyg = polyg->Next;
      }
    return new_geom;
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaCastGeomCollToXY (gaiaGeomCollPtr geom)
{
/* clones a GEOMETRYCOLLECTION converting to XY-dimensions */
    int ib;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaLinestringPtr new_line;
    gaiaPolygonPtr polyg;
    gaiaPolygonPtr new_polyg;
    gaiaGeomCollPtr new_geom;
    gaiaRingPtr i_ring;
    gaiaRingPtr o_ring;
    if (!geom)
	return NULL;
    new_geom = gaiaAllocGeomColl ();
    new_geom->Srid = geom->Srid;
    new_geom->DeclaredType = geom->DeclaredType;
    point = geom->FirstPoint;
    while (point)
      {
	  /* copying POINTs */
	  gaiaAddPointToGeomColl (new_geom, point->X, point->Y);
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* copying LINESTRINGs */
	  new_line = gaiaAddLinestringToGeomColl (new_geom, line->Points);
	  gaiaCopyLinestringCoords (new_line, line);
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* copying POLYGONs */
	  i_ring = polyg->Exterior;
	  new_polyg =
	      gaiaAddPolygonToGeomColl (new_geom, i_ring->Points,
					polyg->NumInteriors);
	  o_ring = new_polyg->Exterior;
	  /* copying points for the EXTERIOR RING */
	  gaiaCopyRingCoords (o_ring, i_ring);
	  for (ib = 0; ib < new_polyg->NumInteriors; ib++)
	    {
		/* copying each INTERIOR RING [if any] */
		i_ring = polyg->Interiors + ib;
		o_ring = gaiaAddInteriorRing (new_polyg, ib, i_ring->Points);
		gaiaCopyRingCoords (o_ring, i_ring);
	    }
	  polyg = polyg->Next;
      }
    return new_geom;
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaCastGeomCollToXYZ (gaiaGeomCollPtr geom)
{
/* clones a GEOMETRYCOLLECTION converting to XYZ-dimensions */
    int ib;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaLinestringPtr new_line;
    gaiaPolygonPtr polyg;
    gaiaPolygonPtr new_polyg;
    gaiaGeomCollPtr new_geom;
    gaiaRingPtr i_ring;
    gaiaRingPtr o_ring;
    if (!geom)
	return NULL;
    new_geom = gaiaAllocGeomCollXYZ ();
    new_geom->Srid = geom->Srid;
    new_geom->DeclaredType = geom->DeclaredType;
    point = geom->FirstPoint;
    while (point)
      {
	  /* copying POINTs */
	  gaiaAddPointToGeomCollXYZ (new_geom, point->X, point->Y, point->Z);
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* copying LINESTRINGs */
	  new_line = gaiaAddLinestringToGeomColl (new_geom, line->Points);
	  gaiaCopyLinestringCoords (new_line, line);
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* copying POLYGONs */
	  i_ring = polyg->Exterior;
	  new_polyg =
	      gaiaAddPolygonToGeomColl (new_geom, i_ring->Points,
					polyg->NumInteriors);
	  o_ring = new_polyg->Exterior;
	  /* copying points for the EXTERIOR RING */
	  gaiaCopyRingCoords (o_ring, i_ring);
	  for (ib = 0; ib < new_polyg->NumInteriors; ib++)
	    {
		/* copying each INTERIOR RING [if any] */
		i_ring = polyg->Interiors + ib;
		o_ring = gaiaAddInteriorRing (new_polyg, ib, i_ring->Points);
		gaiaCopyRingCoords (o_ring, i_ring);
	    }
	  polyg = polyg->Next;
      }
    return new_geom;
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaCastGeomCollToXYM (gaiaGeomCollPtr geom)
{
/* clones a GEOMETRYCOLLECTION converting to XYM-dimensions */
    int ib;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaLinestringPtr new_line;
    gaiaPolygonPtr polyg;
    gaiaPolygonPtr new_polyg;
    gaiaGeomCollPtr new_geom;
    gaiaRingPtr i_ring;
    gaiaRingPtr o_ring;
    if (!geom)
	return NULL;
    new_geom = gaiaAllocGeomCollXYM ();
    new_geom->Srid = geom->Srid;
    new_geom->DeclaredType = geom->DeclaredType;
    point = geom->FirstPoint;
    while (point)
      {
	  /* copying POINTs */
	  gaiaAddPointToGeomCollXYM (new_geom, point->X, point->Y, point->M);
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* copying LINESTRINGs */
	  new_line = gaiaAddLinestringToGeomColl (new_geom, line->Points);
	  gaiaCopyLinestringCoords (new_line, line);
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* copying POLYGONs */
	  i_ring = polyg->Exterior;
	  new_polyg =
	      gaiaAddPolygonToGeomColl (new_geom, i_ring->Points,
					polyg->NumInteriors);
	  o_ring = new_polyg->Exterior;
	  /* copying points for the EXTERIOR RING */
	  gaiaCopyRingCoords (o_ring, i_ring);
	  for (ib = 0; ib < new_polyg->NumInteriors; ib++)
	    {
		/* copying each INTERIOR RING [if any] */
		i_ring = polyg->Interiors + ib;
		o_ring = gaiaAddInteriorRing (new_polyg, ib, i_ring->Points);
		gaiaCopyRingCoords (o_ring, i_ring);
	    }
	  polyg = polyg->Next;
      }
    return new_geom;
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaCastGeomCollToXYZM (gaiaGeomCollPtr geom)
{
/* clones a GEOMETRYCOLLECTION converting to XYZM-dimensions */
    int ib;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaLinestringPtr new_line;
    gaiaPolygonPtr polyg;
    gaiaPolygonPtr new_polyg;
    gaiaGeomCollPtr new_geom;
    gaiaRingPtr i_ring;
    gaiaRingPtr o_ring;
    if (!geom)
	return NULL;
    new_geom = gaiaAllocGeomCollXYZM ();
    new_geom->Srid = geom->Srid;
    new_geom->DeclaredType = geom->DeclaredType;
    point = geom->FirstPoint;
    while (point)
      {
	  /* copying POINTs */
	  gaiaAddPointToGeomCollXYZM (new_geom, point->X, point->Y, point->Z,
				      point->M);
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* copying LINESTRINGs */
	  new_line = gaiaAddLinestringToGeomColl (new_geom, line->Points);
	  gaiaCopyLinestringCoords (new_line, line);
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* copying POLYGONs */
	  i_ring = polyg->Exterior;
	  new_polyg =
	      gaiaAddPolygonToGeomColl (new_geom, i_ring->Points,
					polyg->NumInteriors);
	  o_ring = new_polyg->Exterior;
	  /* copying points for the EXTERIOR RING */
	  gaiaCopyRingCoords (o_ring, i_ring);
	  for (ib = 0; ib < new_polyg->NumInteriors; ib++)
	    {
		/* copying each INTERIOR RING [if any] */
		i_ring = polyg->Interiors + ib;
		o_ring = gaiaAddInteriorRing (new_polyg, ib, i_ring->Points);
		gaiaCopyRingCoords (o_ring, i_ring);
	    }
	  polyg = polyg->Next;
      }
    return new_geom;
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaAllocGeomColl ()
{
/* GEOMETRYCOLLECTION object constructor */
    gaiaGeomCollPtr p = malloc (sizeof (gaiaGeomColl));
    p->Srid = -1;
    p->endian = ' ';
    p->offset = 0;
    p->FirstPoint = NULL;
    p->LastPoint = NULL;
    p->FirstLinestring = NULL;
    p->LastLinestring = NULL;
    p->FirstPolygon = NULL;
    p->LastPolygon = NULL;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY;
    p->DeclaredType = GAIA_UNKNOWN;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaAllocGeomCollXYZ ()
{
/* GEOMETRYCOLLECTION object constructor */
    gaiaGeomCollPtr p = malloc (sizeof (gaiaGeomColl));
    p->Srid = -1;
    p->endian = ' ';
    p->offset = 0;
    p->FirstPoint = NULL;
    p->LastPoint = NULL;
    p->FirstLinestring = NULL;
    p->LastLinestring = NULL;
    p->FirstPolygon = NULL;
    p->LastPolygon = NULL;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_Z;
    p->DeclaredType = GAIA_UNKNOWN;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaAllocGeomCollXYM ()
{
/* GEOMETRYCOLLECTION object constructor */
    gaiaGeomCollPtr p = malloc (sizeof (gaiaGeomColl));
    p->Srid = -1;
    p->endian = ' ';
    p->offset = 0;
    p->FirstPoint = NULL;
    p->LastPoint = NULL;
    p->FirstLinestring = NULL;
    p->LastLinestring = NULL;
    p->FirstPolygon = NULL;
    p->LastPolygon = NULL;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_M;
    p->DeclaredType = GAIA_UNKNOWN;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaAllocGeomCollXYZM ()
{
/* GEOMETRYCOLLECTION object constructor */
    gaiaGeomCollPtr p = malloc (sizeof (gaiaGeomColl));
    p->Srid = -1;
    p->endian = ' ';
    p->offset = 0;
    p->FirstPoint = NULL;
    p->LastPoint = NULL;
    p->FirstLinestring = NULL;
    p->LastLinestring = NULL;
    p->FirstPolygon = NULL;
    p->LastPolygon = NULL;
    p->MinX = DBL_MAX;
    p->MinY = DBL_MAX;
    p->MaxX = -DBL_MAX;
    p->MaxY = -DBL_MAX;
    p->DimensionModel = GAIA_XY_Z_M;
    p->DeclaredType = GAIA_UNKNOWN;
    p->Next = NULL;
    return p;
}

GAIAGEO_DECLARE void
gaiaFreeGeomColl (gaiaGeomCollPtr p)
{
/* GEOMETRYCOLLECTION object destructor */
    gaiaPointPtr pP;
    gaiaPointPtr pPn;
    gaiaLinestringPtr pL;
    gaiaLinestringPtr pLn;
    gaiaPolygonPtr pA;
    gaiaPolygonPtr pAn;
    if (!p)
	return;
    pP = p->FirstPoint;
    while (pP != NULL)
      {
	  pPn = pP->Next;
	  gaiaFreePoint (pP);
	  pP = pPn;
      }
    pL = p->FirstLinestring;
    while (pL != NULL)
      {
	  pLn = pL->Next;
	  gaiaFreeLinestring (pL);
	  pL = pLn;
      }
    pA = p->FirstPolygon;
    while (pA != NULL)
      {
	  pAn = pA->Next;
	  gaiaFreePolygon (pA);
	  pA = pAn;
      }
    free (p);
}

GAIAGEO_DECLARE void
gaiaAddPointToGeomColl (gaiaGeomCollPtr p, double x, double y)
{
/* adding a POINT to this GEOMETRYCOLLECTION */
    gaiaPointPtr point = gaiaAllocPoint (x, y);
    if (p->FirstPoint == NULL)
	p->FirstPoint = point;
    if (p->LastPoint != NULL)
	p->LastPoint->Next = point;
    p->LastPoint = point;
}

GAIAGEO_DECLARE void
gaiaAddPointToGeomCollXYZ (gaiaGeomCollPtr p, double x, double y, double z)
{
/* adding a POINT to this GEOMETRYCOLLECTION */
    gaiaPointPtr point = gaiaAllocPointXYZ (x, y, z);
    if (p->FirstPoint == NULL)
	p->FirstPoint = point;
    if (p->LastPoint != NULL)
	p->LastPoint->Next = point;
    p->LastPoint = point;
}

GAIAGEO_DECLARE void
gaiaAddPointToGeomCollXYM (gaiaGeomCollPtr p, double x, double y, double m)
{
/* adding a POINT to this GEOMETRYCOLLECTION */
    gaiaPointPtr point = gaiaAllocPointXYM (x, y, m);
    if (p->FirstPoint == NULL)
	p->FirstPoint = point;
    if (p->LastPoint != NULL)
	p->LastPoint->Next = point;
    p->LastPoint = point;
}

GAIAGEO_DECLARE void
gaiaAddPointToGeomCollXYZM (gaiaGeomCollPtr p, double x, double y, double z,
			    double m)
{
/* adding a POINT to this GEOMETRYCOLLECTION */
    gaiaPointPtr point = gaiaAllocPointXYZM (x, y, z, m);
    if (p->FirstPoint == NULL)
	p->FirstPoint = point;
    if (p->LastPoint != NULL)
	p->LastPoint->Next = point;
    p->LastPoint = point;
}

GAIAGEO_DECLARE gaiaLinestringPtr
gaiaAddLinestringToGeomColl (gaiaGeomCollPtr p, int vert)
{
/* adding a LINESTRING to this GEOMETRYCOLLECTION */
    gaiaLinestringPtr line;
    if (p->DimensionModel == GAIA_XY_Z)
	line = gaiaAllocLinestringXYZ (vert);
    else if (p->DimensionModel == GAIA_XY_M)
	line = gaiaAllocLinestringXYM (vert);
    else if (p->DimensionModel == GAIA_XY_Z_M)
	line = gaiaAllocLinestringXYZM (vert);
    else
	line = gaiaAllocLinestring (vert);
    if (p->FirstLinestring == NULL)
	p->FirstLinestring = line;
    if (p->LastLinestring != NULL)
	p->LastLinestring->Next = line;
    p->LastLinestring = line;
    return line;
}

GAIAGEO_DECLARE void
gaiaInsertLinestringInGeomColl (gaiaGeomCollPtr p, gaiaLinestringPtr line)
{
/* adding an existing LINESTRING to this GEOMETRYCOLLECTION */
    if (p->FirstLinestring == NULL)
	p->FirstLinestring = line;
    if (p->LastLinestring != NULL)
	p->LastLinestring->Next = line;
    p->LastLinestring = line;
}

GAIAGEO_DECLARE gaiaPolygonPtr
gaiaAddPolygonToGeomColl (gaiaGeomCollPtr p, int vert, int interiors)
{
/* adding a POLYGON to this GEOMETRYCOLLECTION */
    gaiaPolygonPtr polyg;
    if (p->DimensionModel == GAIA_XY_Z)
	polyg = gaiaAllocPolygonXYZ (vert, interiors);
    else if (p->DimensionModel == GAIA_XY_M)
	polyg = gaiaAllocPolygonXYM (vert, interiors);
    else if (p->DimensionModel == GAIA_XY_Z_M)
	polyg = gaiaAllocPolygonXYZM (vert, interiors);
    else
	polyg = gaiaAllocPolygon (vert, interiors);
    if (p->FirstPolygon == NULL)
	p->FirstPolygon = polyg;
    if (p->LastPolygon != NULL)
	p->LastPolygon->Next = polyg;
    p->LastPolygon = polyg;
    return polyg;
}

GAIAGEO_DECLARE gaiaPolygonPtr
gaiaInsertPolygonInGeomColl (gaiaGeomCollPtr p, gaiaRingPtr ring)
{
/* adding a POLYGON to this GEOMETRYCOLLECTION */
    gaiaPolygonPtr polyg;
    polyg = malloc (sizeof (gaiaPolygon));
    polyg->Exterior = ring;
    polyg->NumInteriors = 0;
    polyg->NextInterior = 0;
    polyg->DimensionModel = ring->DimensionModel;
    polyg->Next = NULL;
    polyg->Interiors = NULL;
    polyg->MinX = DBL_MAX;
    polyg->MinY = DBL_MAX;
    polyg->MaxX = -DBL_MAX;
    polyg->MaxY = -DBL_MAX;
    if (p->FirstPolygon == NULL)
	p->FirstPolygon = polyg;
    if (p->LastPolygon != NULL)
	p->LastPolygon->Next = polyg;
    p->LastPolygon = polyg;
    return polyg;
}

GAIAGEO_DECLARE gaiaRingPtr
gaiaAddInteriorRing (gaiaPolygonPtr p, int pos, int vert)
{
/* adding an interior ring to some polygon */
    gaiaRingPtr pP = p->Interiors + pos;
    pP->Points = vert;
    pP->DimensionModel = p->DimensionModel;
    if (pP->DimensionModel == GAIA_XY_Z)
	pP->Coords = malloc (sizeof (double) * (vert * 3));
    else if (pP->DimensionModel == GAIA_XY_M)
	pP->Coords = malloc (sizeof (double) * (vert * 3));
    else if (pP->DimensionModel == GAIA_XY_Z_M)
	pP->Coords = malloc (sizeof (double) * (vert * 4));
    else
	pP->Coords = malloc (sizeof (double) * (vert * 2));
    return pP;
}

GAIAGEO_DECLARE void
gaiaInsertInteriorRing (gaiaPolygonPtr p, gaiaRingPtr ring)
{
/* adding an interior ring to some polygon */
    gaiaRingPtr hole;
    if (p->NumInteriors == 0)
      {
	  /* this one is the first interior ring */
	  p->NumInteriors++;
	  p->Interiors = malloc (sizeof (gaiaRing));
	  hole = p->Interiors;
      }
    else
      {
	  /* some interior ring is already defined */
	  gaiaRingPtr save = p->Interiors;
	  p->Interiors = malloc (sizeof (gaiaRing) * (p->NumInteriors + 1));
	  memcpy (p->Interiors, save, (sizeof (gaiaRing) * p->NumInteriors));
	  free (save);
	  hole = p->Interiors + p->NumInteriors;
	  p->NumInteriors++;
      }
    hole->Points = ring->Points;
    hole->DimensionModel = p->DimensionModel;
    if (hole->DimensionModel == GAIA_XY_Z)
	hole->Coords = malloc (sizeof (double) * (hole->Points * 3));
    else if (hole->DimensionModel == GAIA_XY_M)
	hole->Coords = malloc (sizeof (double) * (hole->Points * 3));
    else if (hole->DimensionModel == GAIA_XY_Z_M)
	hole->Coords = malloc (sizeof (double) * (hole->Points * 4));
    else
	hole->Coords = malloc (sizeof (double) * (hole->Points * 2));
    gaiaCopyRingCoords (hole, ring);
}

GAIAGEO_DECLARE void
gaiaAddRingToPolyg (gaiaPolygonPtr polyg, gaiaRingPtr ring)
{
/* adds an interior ring to this POLYGON  */
    gaiaRingPtr old_interiors = NULL;
    if (!(polyg->Interiors))
      {
	  /* this one is the first interior ring */
	  polyg->Interiors = ring;
	  polyg->NumInteriors = 1;
      }
    else
      {
	  /* adding another interior ring */
	  old_interiors = polyg->Interiors;
	  polyg->Interiors =
	      malloc (sizeof (gaiaRing) * (polyg->NumInteriors + 1));
	  memcpy (polyg->Interiors, old_interiors,
		  (sizeof (gaiaRing) * polyg->NumInteriors));
	  memcpy (polyg->Interiors + polyg->NumInteriors, ring,
		  sizeof (gaiaRing));
	  (polyg->NumInteriors)++;
	  free (old_interiors);
      }
}

GAIAGEO_DECLARE gaiaDynamicLinePtr
gaiaAllocDynamicLine ()
{
/* DYNAMIC LINE object constructor */
    gaiaDynamicLinePtr p = malloc (sizeof (gaiaDynamicLine));
    p->First = NULL;
    p->Last = NULL;
    return p;
}

GAIAGEO_DECLARE void
gaiaFreeDynamicLine (gaiaDynamicLinePtr p)
{
/* DYNAMIC LINE object destructor */
    gaiaPointPtr pP;
    gaiaPointPtr pPn;
    pP = p->First;
    while (pP != NULL)
      {
	  pPn = pP->Next;
	  gaiaFreePoint (pP);
	  pP = pPn;
      }
    free (p);
}

GAIAGEO_DECLARE gaiaPointPtr
gaiaAppendPointToDynamicLine (gaiaDynamicLinePtr p, double x, double y)
{
/* inserts a new POINT to this DYNAMIC LINE after the last one */
    gaiaPointPtr point = gaiaAllocPoint (x, y);
    point->Prev = p->Last;
    if (p->First == NULL)
	p->First = point;
    if (p->Last != NULL)
	p->Last->Next = point;
    p->Last = point;
    return point;
}

GAIAGEO_DECLARE gaiaPointPtr
gaiaPrependPointToDynamicLine (gaiaDynamicLinePtr p, double x, double y)
{
/* inserts a new POINT to this DYNAMIC LINE before the first one */
    gaiaPointPtr point = gaiaAllocPoint (x, y);
    point->Next = p->First;
    if (p->Last == NULL)
	p->Last = point;
    if (p->First != NULL)
	p->First->Prev = point;
    p->First = point;
    return point;
}

GAIAGEO_DECLARE gaiaPointPtr
gaiaDynamicLineInsertAfter (gaiaDynamicLinePtr p, gaiaPointPtr pt, double x,
			    double y)
{
/* inserts a new POINT to this DYNAMIC LINE after the referenced POINT */
    gaiaPointPtr point = gaiaAllocPoint (x, y);
    point->Prev = pt;
    point->Next = pt->Next;
    if (pt->Next)
	pt->Next->Prev = point;
    pt->Next = point;
    if (pt == p->Last)
	p->Last = point;
    return point;
}

GAIAGEO_DECLARE gaiaPointPtr
gaiaDynamicLineInsertBefore (gaiaDynamicLinePtr p, gaiaPointPtr pt, double x,
			     double y)
{
/* inserts a new POINT to this DYNAMIC LINE before the referenced POINT */
    gaiaPointPtr point = gaiaAllocPoint (x, y);
    point->Next = pt;
    point->Prev = pt->Prev;
    if (pt->Prev)
	pt->Prev->Next = point;
    pt->Prev = point;
    if (pt == p->First)
	p->First = point;
    return point;
}

GAIAGEO_DECLARE void
gaiaDynamicLineDeletePoint (gaiaDynamicLinePtr p, gaiaPointPtr pt)
{
/* deletes a POINT from this DYNAMIC LINE */
    if (pt->Prev)
	pt->Prev->Next = pt->Next;
    if (pt->Next)
	pt->Next->Prev = pt->Prev;
    if (pt == p->First)
	p->First = pt->Next;
    if (pt == p->Last)
	p->Last = pt->Prev;
    gaiaFreePoint (pt);
}

GAIAGEO_DECLARE gaiaDynamicLinePtr
gaiaCloneDynamicLine (gaiaDynamicLinePtr org)
{
/* creates a new line obtained by simply copying the current one */
    gaiaPointPtr pt;
    gaiaDynamicLinePtr dst = gaiaAllocDynamicLine ();
    pt = org->First;
    while (pt)
      {
	  gaiaAppendPointToDynamicLine (dst, pt->X, pt->Y);
	  pt = pt->Next;
      }
    return dst;
}

GAIAGEO_DECLARE gaiaDynamicLinePtr
gaiaReverseDynamicLine (gaiaDynamicLinePtr org)
{
/* creates a new line obtained by inverting the current one */
    gaiaPointPtr pt;
    gaiaDynamicLinePtr dst = gaiaAllocDynamicLine ();
    pt = org->Last;
    while (pt)
      {
	  gaiaAppendPointToDynamicLine (dst, pt->X, pt->Y);
	  pt = pt->Prev;
      }
    return dst;
}

GAIAGEO_DECLARE gaiaDynamicLinePtr
gaiaDynamicLineSplitBefore (gaiaDynamicLinePtr org, gaiaPointPtr point)
{
/* creates a new line obtained by cutting the current one in two */
    gaiaDynamicLinePtr dst = gaiaAllocDynamicLine ();
    dst->First = org->First;
    dst->Last = point->Prev;
    point->Prev->Next = NULL;
    org->First = point;
    point->Prev = NULL;
    return dst;
}

GAIAGEO_DECLARE gaiaDynamicLinePtr
gaiaDynamicLineSplitAfter (gaiaDynamicLinePtr org, gaiaPointPtr point)
{
/* creates a new line obtained by cutting the current one in two */
    gaiaDynamicLinePtr dst = gaiaAllocDynamicLine ();
    dst->First = point->Next;
    dst->Last = org->Last;
    point->Next->Prev = NULL;
    org->Last = point;
    point->Next = NULL;
    return dst;
}

GAIAGEO_DECLARE gaiaDynamicLinePtr
gaiaDynamicLineJoinAfter (gaiaDynamicLinePtr org, gaiaPointPtr point,
			  gaiaDynamicLinePtr toJoin)
{
/* creates a new line obtained by joining the current one with another one */
    gaiaPointPtr pt;
    gaiaDynamicLinePtr dst = gaiaAllocDynamicLine ();
    pt = org->First;
    while (pt)
      {
	  /* inserting the first slice since the delimiting POINT included */
	  gaiaAppendPointToDynamicLine (dst, pt->X, pt->Y);
	  if (pt == point)
	      break;
	  pt = pt->Next;
      }
    pt = toJoin->First;
    while (pt)
      {
	  /* inserting the other line */
	  gaiaAppendPointToDynamicLine (dst, pt->X, pt->Y);
	  pt = pt->Next;
      }
    pt = point->Next;
    while (pt)
      {
	  /* inserting the second slice after the delimiting POINT */
	  gaiaAppendPointToDynamicLine (dst, pt->X, pt->Y);
	  pt = pt->Next;
      }
    return dst;
}

GAIAGEO_DECLARE gaiaDynamicLinePtr
gaiaDynamicLineJoinBefore (gaiaDynamicLinePtr org, gaiaPointPtr point,
			   gaiaDynamicLinePtr toJoin)
{
/* creates a new line obtained by joining the current one with another one */
    gaiaPointPtr pt;
    gaiaDynamicLinePtr dst = gaiaAllocDynamicLine ();
    pt = org->First;
    while (pt)
      {
	  /* inserting the first slice since the delimiting POINT excluded */
	  if (pt == point)
	      break;
	  gaiaAppendPointToDynamicLine (dst, pt->X, pt->Y);
	  pt = pt->Next;
      }
    pt = toJoin->First;
    while (pt)
      {
	  /* inserting the other line */
	  gaiaAppendPointToDynamicLine (dst, pt->X, pt->Y);
	  pt = pt->Next;
      }
    pt = point;
    while (pt)
      {
	  /* inserting the second slice beginning from the delimiting POINT */
	  gaiaAppendPointToDynamicLine (dst, pt->X, pt->Y);
	  pt = pt->Next;
      }
    return dst;
}

GAIAGEO_DECLARE gaiaPointPtr
gaiaDynamicLineFindByCoords (gaiaDynamicLinePtr p, double x, double y)
{
/* finds a POINT inside this DYNAMIC LINE */
    gaiaPointPtr pP;
    pP = p->First;
    while (pP != NULL)
      {
	  if (pP->X == x && pP->Y == y)
	      return pP;
	  pP = pP->Next;
      }
    return NULL;
}

GAIAGEO_DECLARE gaiaPointPtr
gaiaDynamicLineFindByPos (gaiaDynamicLinePtr p, int pos)
{
/* finds a POINT inside this DYNAMIC LINE */
    int n = 0;
    gaiaPointPtr pP;
    pP = p->First;
    while (pP != NULL)
      {
	  if (pos == n)
	      return pP;
	  n++;
	  pP = pP->Next;
      }
    return NULL;
}

GAIAGEO_DECLARE gaiaDynamicLinePtr
gaiaCreateDynamicLine (double *coords, int points)
{
/* creates a DynamicLine from an array of coordinates */
    int iv;
    double x;
    double y;
    gaiaDynamicLinePtr line = gaiaAllocDynamicLine ();
    for (iv = 0; iv < points; iv++)
      {
	  gaiaGetPoint (coords, iv, &x, &y);
	  gaiaAppendPointToDynamicLine (line, x, y);
      }
    return line;
}

GAIAGEO_DECLARE void
gaiaMbrLinestring (gaiaLinestringPtr line)
{
/* computes the MBR for this linestring */
    int iv;
    double x;
    double y;
    double z;
    double m;
    line->MinX = DBL_MAX;
    line->MinY = DBL_MAX;
    line->MaxX = -DBL_MAX;
    line->MaxY = -DBL_MAX;
    for (iv = 0; iv < line->Points; iv++)
      {
	  if (line->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (line->Coords, iv, &x, &y, &z);
	    }
	  else if (line->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (line->Coords, iv, &x, &y, &m);
	    }
	  else if (line->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (line->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (line->Coords, iv, &x, &y);
	    }
	  if (x < line->MinX)
	      line->MinX = x;
	  if (y < line->MinY)
	      line->MinY = y;
	  if (x > line->MaxX)
	      line->MaxX = x;
	  if (y > line->MaxY)
	      line->MaxY = y;
      }
}

GAIAGEO_DECLARE void
gaiaMbrRing (gaiaRingPtr rng)
{
/* computes the MBR for this ring */
    int iv;
    double x;
    double y;
    double z;
    double m;
    rng->MinX = DBL_MAX;
    rng->MinY = DBL_MAX;
    rng->MaxX = -DBL_MAX;
    rng->MaxY = -DBL_MAX;
    for (iv = 0; iv < rng->Points; iv++)
      {
	  if (rng->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (rng->Coords, iv, &x, &y, &z);
	    }
	  else if (rng->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (rng->Coords, iv, &x, &y, &m);
	    }
	  else if (rng->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (rng->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (rng->Coords, iv, &x, &y);
	    }
	  if (x < rng->MinX)
	      rng->MinX = x;
	  if (y < rng->MinY)
	      rng->MinY = y;
	  if (x > rng->MaxX)
	      rng->MaxX = x;
	  if (y > rng->MaxY)
	      rng->MaxY = y;
      }
}

GAIAGEO_DECLARE void
gaiaMbrPolygon (gaiaPolygonPtr polyg)
{
/* computes the MBR for this polygon */
    gaiaRingPtr rng;
    polyg->MinX = DBL_MAX;
    polyg->MinY = DBL_MAX;
    polyg->MaxX = -DBL_MAX;
    polyg->MaxY = -DBL_MAX;
    rng = polyg->Exterior;
    gaiaMbrRing (rng);
    if (rng->MinX < polyg->MinX)
	polyg->MinX = rng->MinX;
    if (rng->MinY < polyg->MinY)
	polyg->MinY = rng->MinY;
    if (rng->MaxX > polyg->MaxX)
	polyg->MaxX = rng->MaxX;
    if (rng->MaxY > polyg->MaxY)
	polyg->MaxY = rng->MaxY;
}

GAIAGEO_DECLARE void
gaiaMbrGeometry (gaiaGeomCollPtr geom)
{
/* computes the MBR for this geometry */
    gaiaPointPtr point = NULL;
    gaiaLinestringPtr line = NULL;
    gaiaPolygonPtr polyg = NULL;
    geom->MinX = DBL_MAX;
    geom->MinY = DBL_MAX;
    geom->MaxX = -DBL_MAX;
    geom->MaxY = -DBL_MAX;
    point = geom->FirstPoint;
    while (point)
      {
	  if (point->X < geom->MinX)
	      geom->MinX = point->X;
	  if (point->Y < geom->MinY)
	      geom->MinY = point->Y;
	  if (point->X > geom->MaxX)
	      geom->MaxX = point->X;
	  if (point->Y > geom->MaxY)
	      geom->MaxY = point->Y;
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  gaiaMbrLinestring (line);
	  if (line->MinX < geom->MinX)
	      geom->MinX = line->MinX;
	  if (line->MinY < geom->MinY)
	      geom->MinY = line->MinY;
	  if (line->MaxX > geom->MaxX)
	      geom->MaxX = line->MaxX;
	  if (line->MaxY > geom->MaxY)
	      geom->MaxY = line->MaxY;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  gaiaMbrPolygon (polyg);
	  if (polyg->MinX < geom->MinX)
	      geom->MinX = polyg->MinX;
	  if (polyg->MinY < geom->MinY)
	      geom->MinY = polyg->MinY;
	  if (polyg->MaxX > geom->MaxX)
	      geom->MaxX = polyg->MaxX;
	  if (polyg->MaxY > geom->MaxY)
	      geom->MaxY = polyg->MaxY;
	  polyg = polyg->Next;
      }
}

GAIAGEO_DECLARE void
gaiaMRangeLinestring (gaiaLinestringPtr line, double *min, double *max)
{
/* computes the M-range [min/max] for this linestring */
    int iv;
    double x;
    double y;
    double z;
    double m;
    *min = DBL_MAX;
    *max = -DBL_MAX;
    for (iv = 0; iv < line->Points; iv++)
      {
	  m = 0.0;
	  if (line->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (line->Coords, iv, &x, &y, &z);
	    }
	  else if (line->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (line->Coords, iv, &x, &y, &m);
	    }
	  else if (line->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (line->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (line->Coords, iv, &x, &y);
	    }
	  if (m < *min)
	      *min = m;
	  if (m > *max)
	      *max = m;
      }
}

GAIAGEO_DECLARE void
gaiaMRangeRing (gaiaRingPtr rng, double *min, double *max)
{
/* computes the M-range [min/max] for this ring */
    int iv;
    double x;
    double y;
    double z;
    double m;
    *min = DBL_MAX;
    *max = -DBL_MAX;
    for (iv = 0; iv < rng->Points; iv++)
      {
	  m = 0.0;
	  if (rng->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (rng->Coords, iv, &x, &y, &z);
	    }
	  else if (rng->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (rng->Coords, iv, &x, &y, &m);
	    }
	  else if (rng->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (rng->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (rng->Coords, iv, &x, &y);
	    }
	  if (m < *min)
	      *min = m;
	  if (m > *max)
	      *max = m;
      }
}

GAIAGEO_DECLARE void
gaiaMRangePolygon (gaiaPolygonPtr polyg, double *min, double *max)
{
/* computes the M-range [min/max] for this polygon */
    gaiaRingPtr rng;
    int ib;
    double r_min;
    double r_max;
    *min = DBL_MAX;
    *max = -DBL_MAX;
    rng = polyg->Exterior;
    gaiaMRangeRing (rng, &r_min, &r_max);
    if (r_min < *min)
	*min = r_min;
    if (r_max > *max)
	*max = r_max;
    for (ib = 0; ib < polyg->NumInteriors; ib++)
      {
	  rng = polyg->Interiors + ib;
	  gaiaMRangeRing (rng, &r_min, &r_max);
	  if (r_min < *min)
	      *min = r_min;
	  if (r_max > *max)
	      *max = r_max;
      }
}

GAIAGEO_DECLARE void
gaiaMRangeGeometry (gaiaGeomCollPtr geom, double *min, double *max)
{
/* computes the M-range [min/max] for this geometry */
    gaiaPointPtr point = NULL;
    gaiaLinestringPtr line = NULL;
    gaiaPolygonPtr polyg = NULL;
    double m;
    double r_min;
    double r_max;
    *min = DBL_MAX;
    *max = -DBL_MAX;
    point = geom->FirstPoint;
    while (point)
      {
	  m = 0.0;
	  if (point->DimensionModel == GAIA_XY_M
	      || point->DimensionModel == GAIA_XY_Z_M)
	      m = point->M;
	  if (m < *min)
	      *min = m;
	  if (m > *max)
	      *max = m;
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  gaiaMRangeLinestring (line, &r_min, &r_max);
	  if (r_min < *min)
	      *min = r_min;
	  if (r_max > *max)
	      *max = r_max;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  gaiaMRangePolygon (polyg, &r_min, &r_max);
	  if (r_min < *min)
	      *min = r_min;
	  if (r_max > *max)
	      *max = r_max;
	  polyg = polyg->Next;
      }
}

GAIAGEO_DECLARE void
gaiaZRangeLinestring (gaiaLinestringPtr line, double *min, double *max)
{
/* computes the Z-range [min/max] for this linestring */
    int iv;
    double x;
    double y;
    double z;
    double m;
    *min = DBL_MAX;
    *max = -DBL_MAX;
    for (iv = 0; iv < line->Points; iv++)
      {
	  z = 0.0;
	  m = 0.0;
	  if (line->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (line->Coords, iv, &x, &y, &z);
	    }
	  else if (line->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (line->Coords, iv, &x, &y, &m);
	    }
	  else if (line->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (line->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (line->Coords, iv, &x, &y);
	    }
	  if (z < *min)
	      *min = z;
	  if (z > *max)
	      *max = z;
      }
}

GAIAGEO_DECLARE void
gaiaZRangeRing (gaiaRingPtr rng, double *min, double *max)
{
/* computes the Z-range [min/max] for this ring */
    int iv;
    double x;
    double y;
    double z;
    double m;
    *min = DBL_MAX;
    *max = -DBL_MAX;
    for (iv = 0; iv < rng->Points; iv++)
      {
	  z = 0.0;
	  m = 0.0;
	  if (rng->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (rng->Coords, iv, &x, &y, &z);
	    }
	  else if (rng->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (rng->Coords, iv, &x, &y, &m);
	    }
	  else if (rng->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (rng->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (rng->Coords, iv, &x, &y);
	    }
	  if (m < *min)
	      *min = m;
	  if (m > *max)
	      *max = m;
      }
}

GAIAGEO_DECLARE void
gaiaZRangePolygon (gaiaPolygonPtr polyg, double *min, double *max)
{
/* computes the Z-range [min/max] for this polygon */
    gaiaRingPtr rng;
    int ib;
    double r_min;
    double r_max;
    *min = DBL_MAX;
    *max = -DBL_MAX;
    rng = polyg->Exterior;
    gaiaZRangeRing (rng, &r_min, &r_max);
    if (r_min < *min)
	*min = r_min;
    if (r_max > *max)
	*max = r_max;
    for (ib = 0; ib < polyg->NumInteriors; ib++)
      {
	  rng = polyg->Interiors + ib;
	  gaiaZRangeRing (rng, &r_min, &r_max);
	  if (r_min < *min)
	      *min = r_min;
	  if (r_max > *max)
	      *max = r_max;
      }
}

GAIAGEO_DECLARE void
gaiaZRangeGeometry (gaiaGeomCollPtr geom, double *min, double *max)
{
/* computes the Z-range [min/max] for this geometry */
    gaiaPointPtr point = NULL;
    gaiaLinestringPtr line = NULL;
    gaiaPolygonPtr polyg = NULL;
    double z;
    double r_min;
    double r_max;
    *min = DBL_MAX;
    *max = -DBL_MAX;
    point = geom->FirstPoint;
    while (point)
      {
	  z = 0.0;
	  if (point->DimensionModel == GAIA_XY_Z
	      || point->DimensionModel == GAIA_XY_Z_M)
	      z = point->Z;
	  if (z < *min)
	      *min = z;
	  if (z > *max)
	      *max = z;
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  gaiaZRangeLinestring (line, &r_min, &r_max);
	  if (r_min < *min)
	      *min = r_min;
	  if (r_max > *max)
	      *max = r_max;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  gaiaZRangePolygon (polyg, &r_min, &r_max);
	  if (r_min < *min)
	      *min = r_min;
	  if (r_max > *max)
	      *max = r_max;
	  polyg = polyg->Next;
      }
}

GAIAGEO_DECLARE int
gaiaDimension (gaiaGeomCollPtr geom)
{
/* determinates the Dimension for this geometry */
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    int n_points = 0;
    int n_linestrings = 0;
    int n_polygons = 0;
    if (!geom)
	return -1;
    point = geom->FirstPoint;
    while (point)
      {
	  /* counts how many points are there */
	  n_points++;
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* counts how many linestrings are there */
	  n_linestrings++;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* counts how many polygons are there */
	  n_polygons++;
	  polyg = polyg->Next;
      }
    if (n_points == 0 && n_linestrings == 0 && n_polygons == 0)
	return -1;
    if (n_points > 0 && n_linestrings == 0 && n_polygons == 0)
	return 0;
    if (n_linestrings > 0 && n_polygons == 0)
	return 1;
    return 2;
}

GAIAGEO_DECLARE int
gaiaGeometryType (gaiaGeomCollPtr geom)
{
/* determinates the Class for this geometry */
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    gaiaRingPtr ring;
    int ib;
    int n_points = 0;
    int n_linestrings = 0;
    int n_polygons = 0;
    int dm = GAIA_XY;
    if (!geom)
	return GAIA_UNKNOWN;
    point = geom->FirstPoint;
    while (point)
      {
	  /* counts how many points are there */
	  n_points++;
	  if (point->DimensionModel == GAIA_XY_Z)
	    {
		if (dm == GAIA_XY)
		    dm = GAIA_XY_Z;
		else if (dm == GAIA_XY_M)
		    dm = GAIA_XY_Z_M;
	    }
	  else if (point->DimensionModel == GAIA_XY_M)
	    {
		if (dm == GAIA_XY)
		    dm = GAIA_XY_M;
		else if (dm == GAIA_XY_Z)
		    dm = GAIA_XY_Z_M;
	    }
	  else if (point->DimensionModel == GAIA_XY_Z_M)
	      dm = GAIA_XY_Z_M;
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* counts how many linestrings are there */
	  n_linestrings++;
	  if (line->DimensionModel == GAIA_XY_Z)
	    {
		if (dm == GAIA_XY)
		    dm = GAIA_XY_Z;
		else if (dm == GAIA_XY_M)
		    dm = GAIA_XY_Z_M;
	    }
	  else if (line->DimensionModel == GAIA_XY_M)
	    {
		if (dm == GAIA_XY)
		    dm = GAIA_XY_M;
		else if (dm == GAIA_XY_Z)
		    dm = GAIA_XY_Z_M;
	    }
	  else if (line->DimensionModel == GAIA_XY_Z_M)
	      dm = GAIA_XY_Z_M;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* counts how many polygons are there */
	  n_polygons++;
	  ring = polyg->Exterior;
	  if (ring->DimensionModel == GAIA_XY_Z)
	    {
		if (dm == GAIA_XY)
		    dm = GAIA_XY_Z;
		else if (dm == GAIA_XY_M)
		    dm = GAIA_XY_Z_M;
	    }
	  else if (ring->DimensionModel == GAIA_XY_M)
	    {
		if (dm == GAIA_XY)
		    dm = GAIA_XY_M;
		else if (dm == GAIA_XY_Z)
		    dm = GAIA_XY_Z_M;
	    }
	  else if (ring->DimensionModel == GAIA_XY_Z_M)
	      dm = GAIA_XY_Z_M;
	  for (ib = 0; ib < polyg->NumInteriors; ib++)
	    {
		ring = polyg->Interiors + ib;
		if (ring->DimensionModel == GAIA_XY_Z)
		  {
		      if (dm == GAIA_XY)
			  dm = GAIA_XY_Z;
		      else if (dm == GAIA_XY_M)
			  dm = GAIA_XY_Z_M;
		  }
		else if (ring->DimensionModel == GAIA_XY_M)
		  {
		      if (dm == GAIA_XY)
			  dm = GAIA_XY_M;
		      else if (dm == GAIA_XY_Z)
			  dm = GAIA_XY_Z_M;
		  }
		else if (ring->DimensionModel == GAIA_XY_Z_M)
		    dm = GAIA_XY_Z_M;
	    }
	  polyg = polyg->Next;
      }
    if (n_points == 0 && n_linestrings == 0 && n_polygons == 0)
	return GAIA_UNKNOWN;
    if (n_points == 1 && n_linestrings == 0 && n_polygons == 0)
      {
	  if (geom->DeclaredType == GAIA_MULTIPOINT)
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_MULTIPOINTZ;
		if (dm == GAIA_XY_M)
		    return GAIA_MULTIPOINTM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_MULTIPOINTZM;
		else
		    return GAIA_MULTIPOINT;
	    }
	  else if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_GEOMETRYCOLLECTIONZ;
		if (dm == GAIA_XY_M)
		    return GAIA_GEOMETRYCOLLECTIONM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_GEOMETRYCOLLECTIONZM;
		else
		    return GAIA_GEOMETRYCOLLECTION;
	    }
	  else
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_POINTZ;
		if (dm == GAIA_XY_M)
		    return GAIA_POINTM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_POINTZM;
		else
		    return GAIA_POINT;
	    }
      }
    if (n_points > 0 && n_linestrings == 0 && n_polygons == 0)
      {
	  if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_GEOMETRYCOLLECTIONZ;
		if (dm == GAIA_XY_M)
		    return GAIA_GEOMETRYCOLLECTIONM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_GEOMETRYCOLLECTIONZM;
		else
		    return GAIA_GEOMETRYCOLLECTION;
	    }
	  else
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_MULTIPOINTZ;
		if (dm == GAIA_XY_M)
		    return GAIA_MULTIPOINTM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_MULTIPOINTZM;
		else
		    return GAIA_MULTIPOINT;
	    }
      }
    if (n_points == 0 && n_linestrings == 1 && n_polygons == 0)
      {
	  if (geom->DeclaredType == GAIA_MULTILINESTRING)
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_MULTILINESTRINGZ;
		if (dm == GAIA_XY_M)
		    return GAIA_MULTILINESTRINGM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_MULTILINESTRINGZM;
		else
		    return GAIA_MULTILINESTRING;
	    }
	  else if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_GEOMETRYCOLLECTIONZ;
		if (dm == GAIA_XY_M)
		    return GAIA_GEOMETRYCOLLECTIONM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_GEOMETRYCOLLECTIONZM;
		else
		    return GAIA_GEOMETRYCOLLECTION;
	    }
	  else
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_LINESTRINGZ;
		if (dm == GAIA_XY_M)
		    return GAIA_LINESTRINGM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_LINESTRINGZM;
		else
		    return GAIA_LINESTRING;
	    }
      }
    if (n_points == 0 && n_linestrings > 0 && n_polygons == 0)
      {
	  if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_GEOMETRYCOLLECTIONZ;
		if (dm == GAIA_XY_M)
		    return GAIA_GEOMETRYCOLLECTIONM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_GEOMETRYCOLLECTIONZM;
		else
		    return GAIA_GEOMETRYCOLLECTION;
	    }
	  else
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_MULTILINESTRINGZ;
		if (dm == GAIA_XY_M)
		    return GAIA_MULTILINESTRINGM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_MULTILINESTRINGZM;
		else
		    return GAIA_MULTILINESTRING;
	    }
      }
    if (n_points == 0 && n_linestrings == 0 && n_polygons == 1)
      {
	  if (geom->DeclaredType == GAIA_MULTIPOLYGON)
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_MULTIPOLYGONZ;
		if (dm == GAIA_XY_M)
		    return GAIA_MULTIPOLYGONM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_MULTIPOLYGONZM;
		else
		    return GAIA_MULTIPOLYGON;
	    }
	  else if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_GEOMETRYCOLLECTIONZ;
		if (dm == GAIA_XY_M)
		    return GAIA_GEOMETRYCOLLECTIONM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_GEOMETRYCOLLECTIONZM;
		else
		    return GAIA_GEOMETRYCOLLECTION;
	    }
	  else
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_POLYGONZ;
		if (dm == GAIA_XY_M)
		    return GAIA_POLYGONM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_POLYGONZM;
		else
		    return GAIA_POLYGON;
	    }
      }
    if (n_points == 0 && n_linestrings == 0 && n_polygons > 0)
      {
	  if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_GEOMETRYCOLLECTIONZ;
		if (dm == GAIA_XY_M)
		    return GAIA_GEOMETRYCOLLECTIONM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_GEOMETRYCOLLECTIONZM;
		else
		    return GAIA_GEOMETRYCOLLECTION;
	    }
	  else
	    {
		if (dm == GAIA_XY_Z)
		    return GAIA_MULTIPOLYGONZ;
		if (dm == GAIA_XY_M)
		    return GAIA_MULTIPOLYGONM;
		if (dm == GAIA_XY_Z_M)
		    return GAIA_MULTIPOLYGONZM;
		else
		    return GAIA_MULTIPOLYGON;
	    }
      }
    if (dm == GAIA_XY_Z)
	return GAIA_GEOMETRYCOLLECTIONZ;
    if (dm == GAIA_XY_M)
	return GAIA_GEOMETRYCOLLECTIONM;
    if (dm == GAIA_XY_Z_M)
	return GAIA_GEOMETRYCOLLECTIONZM;
    else
	return GAIA_GEOMETRYCOLLECTION;
}

GAIAGEO_DECLARE int
gaiaGeometryAliasType (gaiaGeomCollPtr geom)
{
/* determinates the AliasClass for this geometry */
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    int n_points = 0;
    int n_linestrings = 0;
    int n_polygons = 0;
    if (!geom)
	return GAIA_UNKNOWN;
    point = geom->FirstPoint;
    while (point)
      {
	  /* counts how many points are there */
	  n_points++;
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* counts how many linestrings are there */
	  n_linestrings++;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* counts how many polygons are there */
	  n_polygons++;
	  polyg = polyg->Next;
      }
    if (n_points == 0 && n_linestrings == 0 && n_polygons == 0)
	return GAIA_UNKNOWN;
    if (n_points == 1 && n_linestrings == 0 && n_polygons == 0)
      {
	  if (geom->DeclaredType == GAIA_MULTIPOINT)
	      return GAIA_MULTIPOINT;
	  else if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	      return GAIA_GEOMETRYCOLLECTION;
	  else
	      return GAIA_POINT;
      }
    if (n_points >= 1 && n_linestrings == 0 && n_polygons == 0)
      {
	  if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	      return GAIA_GEOMETRYCOLLECTION;
	  else
	      return GAIA_MULTIPOINT;
      }
    if (n_points == 0 && n_linestrings == 1 && n_polygons == 0)
      {
	  if (geom->DeclaredType == GAIA_MULTILINESTRING)
	      return GAIA_MULTILINESTRING;
	  else if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	      return GAIA_GEOMETRYCOLLECTION;
	  else
	      return GAIA_LINESTRING;
      }
    if (n_points == 0 && n_linestrings >= 1 && n_polygons == 0)
      {
	  if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	      return GAIA_GEOMETRYCOLLECTION;
	  else
	      return GAIA_MULTILINESTRING;
      }
    if (n_points == 0 && n_linestrings == 0 && n_polygons == 1)
      {
	  if (geom->DeclaredType == GAIA_MULTIPOLYGON)
	      return GAIA_MULTIPOLYGON;
	  else if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	      return GAIA_GEOMETRYCOLLECTION;
	  else
	      return GAIA_POLYGON;
      }
    if (n_points == 0 && n_linestrings == 0 && n_polygons >= 1)
      {
	  if (geom->DeclaredType == GAIA_GEOMETRYCOLLECTION)
	      return GAIA_GEOMETRYCOLLECTION;
	  else
	      return GAIA_MULTIPOLYGON;
      }
    return GAIA_GEOMETRYCOLLECTION;
}

GAIAGEO_DECLARE int
gaiaIsEmpty (gaiaGeomCollPtr geom)
{
/* checks if this GEOMETRYCOLLECTION is an empty one */
    if (!geom)
	return 1;
    if (geom->FirstPoint != NULL)
      {
	  /* there is at least one point */
	  return 0;
      }
    if (geom->FirstLinestring != NULL)
      {
	  /* there is at least one linestring */
	  return 0;
      }
    if (geom->FirstPolygon != NULL)
      {
	  /* there is at least one polygon */
	  return 0;
      }
    return 1;
}

GAIAGEO_DECLARE int
gaiaIsClosed (gaiaLinestringPtr line)
{
/* checks if this linestring is a closed one */
    double x0;
    double y0;
    double x1;
    double y1;
    double z;
    double m;
    if (!line)
	return 0;
    if (line->Points < 3)
	return 0;
    if (line->DimensionModel == GAIA_XY_Z)
      {
	  gaiaGetPointXYZ (line->Coords, 0, &x0, &y0, &z);
      }
    else if (line->DimensionModel == GAIA_XY_M)
      {
	  gaiaGetPointXYM (line->Coords, 0, &x0, &y0, &m);
      }
    else if (line->DimensionModel == GAIA_XY_Z_M)
      {
	  gaiaGetPointXYZM (line->Coords, 0, &x0, &y0, &z, &m);
      }
    else
      {
	  gaiaGetPoint (line->Coords, 0, &x0, &y0);
      }
    if (line->DimensionModel == GAIA_XY_Z)
      {
	  gaiaGetPointXYZ (line->Coords, (line->Points - 1), &x1, &y1, &z);
      }
    else if (line->DimensionModel == GAIA_XY_M)
      {
	  gaiaGetPointXYM (line->Coords, (line->Points - 1), &x1, &y1, &m);
      }
    else if (line->DimensionModel == GAIA_XY_Z_M)
      {
	  gaiaGetPointXYZM (line->Coords, (line->Points - 1), &x1, &y1, &z, &m);
      }
    else
      {
	  gaiaGetPoint (line->Coords, (line->Points - 1), &x1, &y1);
      }
    if (x0 == x1 && y0 == y1)
	return 1;
    return 0;
}

GAIAGEO_DECLARE int
gaiaMbrsEqual (gaiaGeomCollPtr mbr1, gaiaGeomCollPtr mbr2)
{
/* 
/ checks if two MBRs are identicals
/
/ returns 1 if TRUE
/ 0 if FALSE
*/
    if (mbr1->MinX != mbr2->MinX)
	return 0;
    if (mbr1->MinY != mbr2->MinY)
	return 0;
    if (mbr1->MaxX != mbr2->MaxX)
	return 0;
    if (mbr1->MaxY != mbr2->MaxY)
	return 0;
    return 1;
}

GAIAGEO_DECLARE int
gaiaMbrsDisjoint (gaiaGeomCollPtr mbr1, gaiaGeomCollPtr mbr2)
{
/* 
/ checks if two MBRs are disjoined
/
/ returns 1 if TRUE
/ 0 if FALSE
*/
    if (mbr1->MinX >= mbr2->MaxX)
	return 1;
    if (mbr1->MinY >= mbr2->MaxY)
	return 1;
    if (mbr1->MaxX <= mbr2->MinX)
	return 1;
    if (mbr1->MaxY <= mbr2->MinY)
	return 1;
    if (mbr2->MinX >= mbr1->MaxX)
	return 1;
    if (mbr2->MinY >= mbr1->MaxY)
	return 1;
    if (mbr2->MaxX <= mbr1->MinX)
	return 1;
    if (mbr2->MaxY <= mbr1->MinY)
	return 1;
    return 0;
}

GAIAGEO_DECLARE int
gaiaMbrsTouches (gaiaGeomCollPtr mbr1, gaiaGeomCollPtr mbr2)
{
/* 
/ checks if two MBRs touches
/
/ returns 1 if TRUE
/ 0 if FALSE
*/
    if (mbr1->MinX == mbr2->MinX)
	return 1;
    if (mbr1->MinY == mbr2->MinY)
	return 1;
    if (mbr1->MaxX == mbr2->MaxX)
	return 1;
    if (mbr1->MaxY == mbr2->MaxY)
	return 1;
    return 0;
}

GAIAGEO_DECLARE int
gaiaMbrsIntersects (gaiaGeomCollPtr mbr1, gaiaGeomCollPtr mbr2)
{
/* 
/ checks if two MBRs intersects
/
/ returns 1 if TRUE
/ 0 if FALSE
*/
    if (gaiaMbrsDisjoint (mbr1, mbr2))
	return 0;
    return 1;
}

GAIAGEO_DECLARE int
gaiaMbrsOverlaps (gaiaGeomCollPtr mbr1, gaiaGeomCollPtr mbr2)
{
/* 
/ checks if two MBRs overlaps
/
/ returns 1 if TRUE
/ 0 if FALSE
*/
    if (gaiaMbrsDisjoint (mbr1, mbr2))
	return 0;
    if (mbr1->MinX >= mbr2->MinX && mbr1->MinX <= mbr2->MaxX)
	return 1;
    if (mbr1->MaxX >= mbr2->MinX && mbr1->MaxX <= mbr2->MaxX)
	return 1;
    if (mbr1->MinY >= mbr2->MinY && mbr1->MinY <= mbr2->MaxY)
	return 1;
    if (mbr1->MaxY >= mbr2->MinY && mbr1->MaxY <= mbr2->MaxY)
	return 1;
    return 0;
}

GAIAGEO_DECLARE int
gaiaMbrsContains (gaiaGeomCollPtr mbr1, gaiaGeomCollPtr mbr2)
{
/* 
/ checks if MBR-1 completely contains MBR-2
/
/ returns 1 if TRUE
/ 0 if FALSE
*/
    int ok_1 = 0;
    int ok_2 = 0;
    int ok_3 = 0;
    int ok_4 = 0;
    if (mbr2->MinX >= mbr1->MinX && mbr2->MinX <= mbr1->MaxX)
	ok_1 = 1;
    if (mbr2->MaxX >= mbr1->MinX && mbr2->MaxX <= mbr1->MaxX)
	ok_2 = 1;
    if (mbr2->MinY >= mbr1->MinY && mbr2->MinY <= mbr1->MaxY)
	ok_3 = 1;
    if (mbr2->MaxY >= mbr1->MinY && mbr2->MaxY <= mbr1->MaxY)
	ok_4 = 1;
    if (ok_1 && ok_2 && ok_3 && ok_4)
	return 1;
    return 0;
}

GAIAGEO_DECLARE int
gaiaMbrsWithin (gaiaGeomCollPtr mbr1, gaiaGeomCollPtr mbr2)
{
/* 
/ checks if MBR-2 completely contains MBR-1
/
/ returns 1 if TRUE
/ 0 if FALSE
*/
    int ok_1 = 0;
    int ok_2 = 0;
    int ok_3 = 0;
    int ok_4 = 0;
    if (mbr1->MinX >= mbr2->MinX && mbr1->MinX <= mbr2->MaxX)
	ok_1 = 1;
    if (mbr1->MaxX >= mbr2->MinX && mbr1->MaxX <= mbr2->MaxX)
	ok_2 = 1;
    if (mbr1->MinY >= mbr2->MinY && mbr1->MinY <= mbr2->MaxY)
	ok_3 = 1;
    if (mbr1->MaxY >= mbr2->MinY && mbr1->MaxY <= mbr2->MaxY)
	ok_4 = 1;
    if (ok_1 && ok_2 && ok_3 && ok_4)
	return 1;
    return 0;
}

GAIAGEO_DECLARE void
gaiaMakePoint (double x, double y, int srid, unsigned char **result, int *size)
{
/* build a Blob encoded Geometry representing a POINT */
    unsigned char *ptr;
    int endian_arch = gaiaEndianArch ();
/* computing the Blob size and then allocating it */
    *size = 44;			/* header size */
    *size += (sizeof (double) * 2);	/* [x,y] coords */
    *result = malloc (*size);
    ptr = *result;
/* setting the Blob value */
    *ptr = GAIA_MARK_START;	/* START signatue */
    *(ptr + 1) = GAIA_LITTLE_ENDIAN;	/* byte ordering */
    gaiaExport32 (ptr + 2, srid, 1, endian_arch);	/* the SRID */
    gaiaExport64 (ptr + 6, x, 1, endian_arch);	/* MBR - minimun X */
    gaiaExport64 (ptr + 14, y, 1, endian_arch);	/* MBR - minimun Y */
    gaiaExport64 (ptr + 22, x, 1, endian_arch);	/* MBR - maximun X */
    gaiaExport64 (ptr + 30, y, 1, endian_arch);	/* MBR - maximun Y */
    *(ptr + 38) = GAIA_MARK_MBR;	/* MBR signature */
    gaiaExport32 (ptr + 39, GAIA_POINT, 1, endian_arch);	/* class POINT */
    gaiaExport64 (ptr + 43, x, 1, endian_arch);	/* X */
    gaiaExport64 (ptr + 51, y, 1, endian_arch);	/* Y */
    *(ptr + 59) = GAIA_MARK_END;	/* END signature */
}

GAIAGEO_DECLARE void
gaiaBuildMbr (double x1, double y1, double x2, double y2, int srid,
	      unsigned char **result, int *size)
{
/* build a Blob encoded Geometry representing an MBR */
    unsigned char *ptr;
    double minx;
    double maxx;
    double miny;
    double maxy;
    int endian_arch = gaiaEndianArch ();
/* computing MinMax coords */
    if (x1 > x2)
      {
	  maxx = x1;
	  minx = x2;
      }
    else
      {
	  minx = x1;
	  maxx = x2;
      }
    if (y1 > y2)
      {
	  maxy = y1;
	  miny = y2;
      }
    else
      {
	  miny = y1;
	  maxy = y2;
      }
/* computing the Blob size and then allocating it */
    *size = 44;			/* header size */
    *size += (8 + ((sizeof (double) * 2) * 5));	/* # rings + # points + [x.y] array - exterior ring */
    *result = malloc (*size);
    ptr = *result;
/* setting the Blob value */
    *ptr = GAIA_MARK_START;	/* START signatue */
    *(ptr + 1) = GAIA_LITTLE_ENDIAN;	/* byte ordering */
    gaiaExport32 (ptr + 2, srid, 1, endian_arch);	/* the SRID */
    gaiaExport64 (ptr + 6, minx, 1, endian_arch);	/* MBR - minimun X */
    gaiaExport64 (ptr + 14, miny, 1, endian_arch);	/* MBR - minimun Y */
    gaiaExport64 (ptr + 22, maxx, 1, endian_arch);	/* MBR - maximun X */
    gaiaExport64 (ptr + 30, maxy, 1, endian_arch);	/* MBR - maximun Y */
    *(ptr + 38) = GAIA_MARK_MBR;	/* MBR signature */
    gaiaExport32 (ptr + 39, GAIA_POLYGON, 1, endian_arch);	/* class POLYGON */
    gaiaExport32 (ptr + 43, 1, 1, endian_arch);	/* # rings */
    gaiaExport32 (ptr + 47, 5, 1, endian_arch);	/* # points - exterior ring */
    ptr += 51;
/* setting Envelope points */
    gaiaExport64 (ptr, minx, 1, endian_arch);
    gaiaExport64 (ptr + 8, miny, 1, endian_arch);
    ptr += 16;
    gaiaExport64 (ptr, maxx, 1, endian_arch);
    gaiaExport64 (ptr + 8, miny, 1, endian_arch);
    ptr += 16;
    gaiaExport64 (ptr, maxx, 1, endian_arch);
    gaiaExport64 (ptr + 8, maxy, 1, endian_arch);
    ptr += 16;
    gaiaExport64 (ptr, minx, 1, endian_arch);
    gaiaExport64 (ptr + 8, maxy, 1, endian_arch);
    ptr += 16;
    gaiaExport64 (ptr, minx, 1, endian_arch);
    gaiaExport64 (ptr + 8, miny, 1, endian_arch);
    ptr += 16;
    *ptr = GAIA_MARK_END;	/* END signature */
}

GAIAGEO_DECLARE void
gaiaBuildCircleMbr (double x, double y, double radius, int srid,
		    unsigned char **result, int *size)
{
/* build a Blob encoded Geometry representing an MBR */
    int sz;
    unsigned char *res = NULL;
    double minx = x - radius;
    double maxx = x + radius;
    double miny = y - radius;
    double maxy = y + radius;
    gaiaBuildMbr (minx, miny, maxx, maxy, srid, &res, &sz);
    if (!res)
      {
	  *result = NULL;
	  *size = 0;
      }
    else
      {
	  *result = res;
	  *size = sz;
      }
}

GAIAGEO_DECLARE void
gaiaBuildFilterMbr (double x1, double y1, double x2, double y2, int mode,
		    unsigned char **result, int *size)
{
/* build a filter for an MBR */
    unsigned char *ptr;
    double minx;
    double maxx;
    double miny;
    double maxy;
    int endian_arch = gaiaEndianArch ();
    char filter = GAIA_FILTER_MBR_WITHIN;
    if (mode == GAIA_FILTER_MBR_CONTAINS)
	filter = GAIA_FILTER_MBR_CONTAINS;
    if (mode == GAIA_FILTER_MBR_INTERSECTS)
	filter = GAIA_FILTER_MBR_INTERSECTS;
    if (mode == GAIA_FILTER_MBR_DECLARE)
	filter = GAIA_FILTER_MBR_DECLARE;
/* computing MinMax coords */
    if (x1 > x2)
      {
	  maxx = x1;
	  minx = x2;
      }
    else
      {
	  minx = x1;
	  maxx = x2;
      }
    if (y1 > y2)
      {
	  maxy = y1;
	  miny = y2;
      }
    else
      {
	  miny = y1;
	  maxy = y2;
      }
/* computing the Blob size and then allocating it */
    *size = 37;			/* MBR filter size */
    *result = malloc (*size);
    ptr = *result;
/* setting the Blob value */
    *ptr = filter;		/* signatue */
    ptr++;
    gaiaExport64 (ptr, minx, 1, endian_arch);	/* MBR - minimun X */
    ptr += 8;
    *ptr = filter;		/* signatue */
    ptr++;
    gaiaExport64 (ptr, miny, 1, endian_arch);	/* MBR - minimun Y */
    ptr += 8;
    *ptr = filter;		/* signatue */
    ptr++;
    gaiaExport64 (ptr, maxx, 1, endian_arch);	/* MBR - maximun X */
    ptr += 8;
    *ptr = filter;		/* signatue */
    ptr++;
    gaiaExport64 (ptr, maxy, 1, endian_arch);	/* MBR - maximun Y */
    ptr += 8;
    *ptr = filter;		/* signatue */
}


GAIAGEO_DECLARE int
gaiaParseFilterMbr (unsigned char *ptr, int size, double *minx, double *miny,
		    double *maxx, double *maxy, int *mode)
{
/* parsing a filter for an MBR */
    char decl_mode;
    int endian_arch = gaiaEndianArch ();
    if (size != 37)
	return 0;		/* cannot be an MBR Filter */
    if (!ptr)
	return 0;		/* cannot be an MBR Filter */
    decl_mode = *(ptr + 0);
    if (decl_mode == GAIA_FILTER_MBR_WITHIN)
	;
    else if (decl_mode == GAIA_FILTER_MBR_CONTAINS)
	;
    else if (decl_mode == GAIA_FILTER_MBR_INTERSECTS)
	;
    else if (decl_mode == GAIA_FILTER_MBR_DECLARE)
	;
    else
	return 0;		/* cannot be an MBR Filter */
    if (*(ptr + 9)
	== decl_mode
	&& *(ptr +
	     18) ==
	decl_mode && *(ptr + 27) == decl_mode && *(ptr + 36) == decl_mode)
	;
    else
	return 0;		/* cannot be an MBR Filter */
    *mode = decl_mode;
    *minx = gaiaImport64 (ptr + 1, 1, endian_arch);
    *miny = gaiaImport64 (ptr + 10, 1, endian_arch);
    *maxx = gaiaImport64 (ptr + 19, 1, endian_arch);
    *maxy = gaiaImport64 (ptr + 28, 1, endian_arch);
    return 1;
}

GAIAGEO_DECLARE int
gaiaGetMbrMinX (const unsigned char *blob, unsigned int size, double *minx)
{
/* returns the MinX coordinate value for a Blob encoded Geometry */
    int little_endian;
    int endian_arch = gaiaEndianArch ();
    if (size < 45)
	return 0;		/* cannot be an internal BLOB WKB geometry */
    if (*(blob + 0) != GAIA_MARK_START)
	return 0;		/* failed to recognize START signature */
    if (*(blob + (size - 1)) != GAIA_MARK_END)
	return 0;		/* failed to recognize END signature */
    if (*(blob + 38) != GAIA_MARK_MBR)
	return 0;		/* failed to recognize MBR signature */
    if (*(blob + 1) == GAIA_LITTLE_ENDIAN)
	little_endian = 1;
    else if (*(blob + 1) == GAIA_BIG_ENDIAN)
	little_endian = 0;
    else
	return 0;		/* unknown encoding; nor litte-endian neither big-endian */
    *minx = gaiaImport64 (blob + 6, little_endian, endian_arch);
    return 1;
}

GAIAGEO_DECLARE int
gaiaGetMbrMaxX (const unsigned char *blob, unsigned int size, double *maxx)
{
/* returns the MaxX coordinate value for a Blob encoded Geometry */
    int little_endian;
    int endian_arch = gaiaEndianArch ();
    if (size < 45)
	return 0;		/* cannot be an internal BLOB WKB geometry */
    if (*(blob + 0) != GAIA_MARK_START)
	return 0;		/* failed to recognize START signature */
    if (*(blob + (size - 1)) != GAIA_MARK_END)
	return 0;		/* failed to recognize END signature */
    if (*(blob + 38) != GAIA_MARK_MBR)
	return 0;		/* failed to recognize MBR signature */
    if (*(blob + 1) == GAIA_LITTLE_ENDIAN)
	little_endian = 1;
    else if (*(blob + 1) == GAIA_BIG_ENDIAN)
	little_endian = 0;
    else
	return 0;		/* unknown encoding; nor litte-endian neither big-endian */
    *maxx = gaiaImport64 (blob + 22, little_endian, endian_arch);
    return 1;
}

GAIAGEO_DECLARE int
gaiaGetMbrMinY (const unsigned char *blob, unsigned int size, double *miny)
{
/* returns the MinY coordinate value for a Blob encoded Geometry */
    int little_endian;
    int endian_arch = gaiaEndianArch ();
    if (size < 45)
	return 0;		/* cannot be an internal BLOB WKB geometry */
    if (*(blob + 0) != GAIA_MARK_START)
	return 0;		/* failed to recognize START signature */
    if (*(blob + (size - 1)) != GAIA_MARK_END)
	return 0;		/* failed to recognize END signature */
    if (*(blob + 38) != GAIA_MARK_MBR)
	return 0;		/* failed to recognize MBR signature */
    if (*(blob + 1) == GAIA_LITTLE_ENDIAN)
	little_endian = 1;
    else if (*(blob + 1) == GAIA_BIG_ENDIAN)
	little_endian = 0;
    else
	return 0;		/* unknown encoding; nor litte-endian neither big-endian */
    *miny = gaiaImport64 (blob + 14, little_endian, endian_arch);
    return 1;
}

GAIAGEO_DECLARE int
gaiaGetMbrMaxY (const unsigned char *blob, unsigned int size, double *maxy)
{
/* returns the MaxY coordinate value for a Blob encoded Geometry */
    int little_endian;
    int endian_arch = gaiaEndianArch ();
    if (size < 45)
	return 0;		/* cannot be an internal BLOB WKB geometry */
    if (*(blob + 0) != GAIA_MARK_START)
	return 0;		/* failed to recognize START signature */
    if (*(blob + (size - 1)) != GAIA_MARK_END)
	return 0;		/* failed to recognize END signature */
    if (*(blob + 38) != GAIA_MARK_MBR)
	return 0;		/* failed to recognize MBR signature */
    if (*(blob + 1) == GAIA_LITTLE_ENDIAN)
	little_endian = 1;
    else if (*(blob + 1) == GAIA_BIG_ENDIAN)
	little_endian = 0;
    else
	return 0;		/* unknown encoding; nor litte-endian neither big-endian */
    *maxy = gaiaImport64 (blob + 30, little_endian, endian_arch);
    return 1;
}

GAIAGEO_DECLARE gaiaGeomCollPtr
gaiaSanitize (gaiaGeomCollPtr geom)
{
/* 
/ sanitizes a GEOMETRYCOLLECTION:
/ - repeated vertices are omitted
/ - ring closure is enforced anyway  
*/
    int iv;
    int ib;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double m = 0.0;
    double first_x;
    double first_y;
    double first_z;
    double first_m;
    double last_x = 0.0;
    double last_y = 0.0;
    double last_z = 0.0;
    double last_m = 0.0;
    int points;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaLinestringPtr new_line;
    gaiaPolygonPtr polyg;
    gaiaPolygonPtr new_polyg;
    gaiaGeomCollPtr new_geom;
    gaiaRingPtr i_ring;
    gaiaRingPtr o_ring;
    if (!geom)
	return NULL;
    if (geom->DimensionModel == GAIA_XY_Z)
	new_geom = gaiaAllocGeomCollXYZ ();
    else if (geom->DimensionModel == GAIA_XY_M)
	new_geom = gaiaAllocGeomCollXYM ();
    else if (geom->DimensionModel == GAIA_XY_Z_M)
	new_geom = gaiaAllocGeomCollXYZM ();
    else
	new_geom = gaiaAllocGeomColl ();
    new_geom->Srid = geom->Srid;
    new_geom->DeclaredType = geom->DeclaredType;
    point = geom->FirstPoint;
    while (point)
      {
	  /* copying POINTs */
	  gaiaAddPointToGeomCollXYZM (new_geom, point->X, point->Y, point->Z,
				      point->M);
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* sanitizing LINESTRINGs */
	  points = 0;
	  for (iv = 0; iv < line->Points; iv++)
	    {
		/* PASS I - checking points */
		z = 0.0;
		m = 0.0;
		if (line->DimensionModel == GAIA_XY_Z)
		  {
		      gaiaGetPointXYZ (line->Coords, iv, &x, &y, &z);
		  }
		else if (line->DimensionModel == GAIA_XY_M)
		  {
		      gaiaGetPointXYM (line->Coords, iv, &x, &y, &m);
		  }
		else if (line->DimensionModel == GAIA_XY_Z_M)
		  {
		      gaiaGetPointXYZM (line->Coords, iv, &x, &y, &z, &m);
		  }
		else
		  {
		      gaiaGetPoint (line->Coords, iv, &x, &y);
		  }
		if (iv > 0)
		  {
		      if (last_x == x && last_y == y && last_z == z)
			  ;
		      else
			  points++;
		  }
		else
		    points++;
		last_x = x;
		last_y = y;
		last_z = z;
		last_m = m;
	    }
	  if (points < 2)
	    {
		/* illegal LINESTRING - copying the original one */
		new_line = gaiaAddLinestringToGeomColl (new_geom, line->Points);
		gaiaCopyLinestringCoords (new_line, line);
	    }
	  else
	    {
		/* valid LINESTRING - sanitizing */
		new_line = gaiaAddLinestringToGeomColl (new_geom, points);
		points = 0;
		for (iv = 0; iv < line->Points; iv++)
		  {
		      /* PASS II - inserting points */
		      z = 0.0;
		      m = 0.0;
		      if (line->DimensionModel == GAIA_XY_Z)
			{
			    gaiaGetPointXYZ (line->Coords, iv, &x, &y, &z);
			}
		      else if (line->DimensionModel == GAIA_XY_M)
			{
			    gaiaGetPointXYM (line->Coords, iv, &x, &y, &m);
			}
		      else if (line->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaGetPointXYZM (line->Coords, iv, &x, &y, &z, &m);
			}
		      else
			{
			    gaiaGetPoint (line->Coords, iv, &x, &y);
			}
		      if (iv > 0)
			{
			    if (last_x == x && last_y == y && last_z == z)
				;
			    else
			      {
				  if (new_line->DimensionModel == GAIA_XY_Z)
				    {
					gaiaSetPointXYZ (new_line->Coords,
							 points, x, y, z);
				    }
				  else if (new_line->DimensionModel ==
					   GAIA_XY_M)
				    {
					gaiaSetPointXYM (new_line->Coords,
							 points, x, y, m);
				    }
				  else if (new_line->DimensionModel ==
					   GAIA_XY_Z_M)
				    {
					gaiaSetPointXYZM (new_line->Coords,
							  points, x, y, z, m);
				    }
				  else
				    {
					gaiaSetPoint (new_line->Coords, points,
						      x, y);
				    }
				  points++;
			      }
			}
		      else
			{
			    if (new_line->DimensionModel == GAIA_XY_Z)
			      {
				  gaiaSetPointXYZ (new_line->Coords, points, x,
						   y, z);
			      }
			    else if (new_line->DimensionModel == GAIA_XY_M)
			      {
				  gaiaSetPointXYM (new_line->Coords, points, x,
						   y, m);
			      }
			    else if (new_line->DimensionModel == GAIA_XY_Z_M)
			      {
				  gaiaSetPointXYZM (new_line->Coords, points, x,
						    y, z, m);
			      }
			    else
			      {
				  gaiaSetPoint (new_line->Coords, points, x, y);
			      }
			    points++;
			}
		      last_x = x;
		      last_y = y;
		      last_z = z;
		      last_m = m;
		  }
	    }
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* copying POLYGONs */
	  i_ring = polyg->Exterior;
	  /* sanitizing EXTERIOR RING */
	  points = 0;
	  for (iv = 0; iv < i_ring->Points; iv++)
	    {
		/* PASS I - checking points */
		z = 0.0;
		m = 0.0;
		if (i_ring->DimensionModel == GAIA_XY_Z)
		  {
		      gaiaGetPointXYZ (i_ring->Coords, iv, &x, &y, &z);
		  }
		else if (i_ring->DimensionModel == GAIA_XY_M)
		  {
		      gaiaGetPointXYM (i_ring->Coords, iv, &x, &y, &m);
		  }
		else if (i_ring->DimensionModel == GAIA_XY_Z_M)
		  {
		      gaiaGetPointXYZM (i_ring->Coords, iv, &x, &y, &z, &m);
		  }
		else
		  {
		      gaiaGetPoint (i_ring->Coords, iv, &x, &y);
		  }
		if (iv > 0)
		  {
		      if (last_x == x && last_y == y && last_z == z)
			  ;
		      else
			  points++;
		  }
		else
		  {
		      first_x = x;
		      first_y = y;
		      first_z = z;
		      first_m = m;
		      points++;
		  }
		last_x = x;
		last_y = y;
		last_z = z;
		last_m = m;
	    }
	  if (last_x == x && last_y == y && last_z == z)
	      ;
	  else
	    {
		/* forcing RING closure */
		points++;
	    }
	  if (points < 4)
	    {
		/* illegal RING - copying the original one */
		new_polyg =
		    gaiaAddPolygonToGeomColl (new_geom, i_ring->Points,
					      polyg->NumInteriors);
		o_ring = new_polyg->Exterior;
		gaiaCopyRingCoords (o_ring, i_ring);
	    }
	  else
	    {
		/* valid RING - sanitizing */
		new_polyg =
		    gaiaAddPolygonToGeomColl (new_geom, points,
					      polyg->NumInteriors);
		o_ring = new_polyg->Exterior;
		points = 0;
		for (iv = 0; iv < i_ring->Points; iv++)
		  {
		      /* PASS II - inserting points */
		      z = 0.0;
		      m = 0.0;
		      if (i_ring->DimensionModel == GAIA_XY_Z)
			{
			    gaiaGetPointXYZ (i_ring->Coords, iv, &x, &y, &z);
			}
		      else if (i_ring->DimensionModel == GAIA_XY_M)
			{
			    gaiaGetPointXYM (i_ring->Coords, iv, &x, &y, &m);
			}
		      else if (i_ring->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaGetPointXYZM (i_ring->Coords, iv, &x, &y, &z,
					      &m);
			}
		      else
			{
			    gaiaGetPoint (i_ring->Coords, iv, &x, &y);
			}
		      if (iv > 0)
			{
			    if (last_x == x && last_y == y && last_z == z)
				;
			    else
			      {
				  if (o_ring->DimensionModel == GAIA_XY_Z)
				    {
					gaiaSetPointXYZ (o_ring->Coords, points,
							 x, y, z);
				    }
				  else if (o_ring->DimensionModel == GAIA_XY_M)
				    {
					gaiaSetPointXYM (o_ring->Coords, points,
							 x, y, m);
				    }
				  else if (o_ring->DimensionModel ==
					   GAIA_XY_Z_M)
				    {
					gaiaSetPointXYZM (o_ring->Coords,
							  points, x, y, z, m);
				    }
				  else
				    {
					gaiaSetPoint (o_ring->Coords, points, x,
						      y);
				    }
				  points++;
			      }
			}
		      else
			{
			    if (o_ring->DimensionModel == GAIA_XY_Z)
			      {
				  gaiaSetPointXYZ (o_ring->Coords, points, x,
						   y, z);
			      }
			    else if (o_ring->DimensionModel == GAIA_XY_M)
			      {
				  gaiaSetPointXYM (o_ring->Coords, points, x,
						   y, m);
			      }
			    else if (o_ring->DimensionModel == GAIA_XY_Z_M)
			      {
				  gaiaSetPointXYZM (o_ring->Coords, points, x,
						    y, z, m);
			      }
			    else
			      {
				  gaiaSetPoint (o_ring->Coords, points, x, y);
			      }
			    points++;
			}
		      last_x = x;
		      last_y = y;
		      last_z = z;
		      last_m = m;
		  }
	    }
	  /* PASS III - forcing RING closure */
	  z = 0.0;
	  m = 0.0;
	  if (i_ring->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (i_ring->Coords, 0, &x, &y, &z);
	    }
	  else if (i_ring->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (i_ring->Coords, 0, &x, &y, &m);
	    }
	  else if (i_ring->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (i_ring->Coords, 0, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (i_ring->Coords, 0, &x, &y);
	    }
	  points = o_ring->Points - 1;
	  if (o_ring->DimensionModel == GAIA_XY_Z)
	    {
		gaiaSetPointXYZ (o_ring->Coords, points, x, y, z);
	    }
	  else if (o_ring->DimensionModel == GAIA_XY_M)
	    {
		gaiaSetPointXYM (o_ring->Coords, points, x, y, m);
	    }
	  else if (o_ring->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaSetPointXYZM (o_ring->Coords, points, x, y, z, m);
	    }
	  else
	    {
		gaiaSetPoint (o_ring->Coords, points, x, y);
	    }
	  for (ib = 0; ib < new_polyg->NumInteriors; ib++)
	    {
		/* copying each INTERIOR RING [if any] */
		i_ring = polyg->Interiors + ib;
		/* sanitizing an INTERIOR RING */
		points = 0;
		for (iv = 0; iv < i_ring->Points; iv++)
		  {
		      /* PASS I - checking points */
		      z = 0.0;
		      m = 0.0;
		      if (i_ring->DimensionModel == GAIA_XY_Z)
			{
			    gaiaGetPointXYZ (i_ring->Coords, iv, &x, &y, &z);
			}
		      else if (i_ring->DimensionModel == GAIA_XY_M)
			{
			    gaiaGetPointXYM (i_ring->Coords, iv, &x, &y, &m);
			}
		      else if (i_ring->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaGetPointXYZM (i_ring->Coords, iv, &x, &y, &z,
					      &m);
			}
		      else
			{
			    gaiaGetPoint (i_ring->Coords, iv, &x, &y);
			}
		      if (iv > 0)
			{
			    if (last_x == x && last_y == y && last_z == z)
				;
			    else
				points++;
			}
		      else
			{
			    first_x = x;
			    first_y = y;
			    first_z = z;
			    first_m = m;
			    points++;
			}
		      last_x = x;
		      last_y = y;
		      last_z = z;
		      last_m = m;
		  }
		if (last_x == x && last_y == y && last_z == z)
		    ;
		else
		  {
		      /* forcing RING closure */
		      points++;
		  }
		if (points < 4)
		  {
		      /* illegal RING - copying the original one */
		      o_ring =
			  gaiaAddInteriorRing (new_polyg, ib, i_ring->Points);
		      gaiaCopyRingCoords (o_ring, i_ring);
		  }
		else
		  {
		      /* valid RING - sanitizing */
		      o_ring = gaiaAddInteriorRing (new_polyg, ib, points);
		      points = 0;
		      for (iv = 0; iv < i_ring->Points; iv++)
			{
			    /* PASS II - inserting points */
			    z = 0.0;
			    m = 0.0;
			    if (i_ring->DimensionModel == GAIA_XY_Z)
			      {
				  gaiaGetPointXYZ (i_ring->Coords, iv, &x, &y,
						   &z);
			      }
			    else if (i_ring->DimensionModel == GAIA_XY_M)
			      {
				  gaiaGetPointXYM (i_ring->Coords, iv, &x, &y,
						   &m);
			      }
			    else if (i_ring->DimensionModel == GAIA_XY_Z_M)
			      {
				  gaiaGetPointXYZM (i_ring->Coords, iv, &x, &y,
						    &z, &m);
			      }
			    else
			      {
				  gaiaGetPoint (i_ring->Coords, iv, &x, &y);
			      }
			    if (iv > 0)
			      {
				  if (last_x == x && last_y == y && last_z == z)
				      ;
				  else
				    {
					if (o_ring->DimensionModel == GAIA_XY_Z)
					  {
					      gaiaSetPointXYZ (o_ring->Coords,
							       points, x, y, z);
					  }
					else if (o_ring->DimensionModel ==
						 GAIA_XY_M)
					  {
					      gaiaSetPointXYM (o_ring->Coords,
							       points, x, y, m);
					  }
					else if (o_ring->DimensionModel ==
						 GAIA_XY_Z_M)
					  {
					      gaiaSetPointXYZM (o_ring->Coords,
								points, x, y, z,
								m);
					  }
					else
					  {
					      gaiaSetPoint (o_ring->Coords,
							    points, x, y);
					  }
					points++;
				    }
			      }
			    else
			      {
				  if (o_ring->DimensionModel == GAIA_XY_Z)
				    {
					gaiaSetPointXYZ (o_ring->Coords, points,
							 x, y, z);
				    }
				  else if (o_ring->DimensionModel == GAIA_XY_M)
				    {
					gaiaSetPointXYM (o_ring->Coords, points,
							 x, y, m);
				    }
				  else if (o_ring->DimensionModel ==
					   GAIA_XY_Z_M)
				    {
					gaiaSetPointXYZM (o_ring->Coords,
							  points, x, y, z, m);
				    }
				  else
				    {
					gaiaSetPoint (o_ring->Coords, points, x,
						      y);
				    }
				  points++;
			      }
			    last_x = x;
			    last_y = y;
			    last_z = z;
			    last_m = m;
			}
		      /* PASS III - forcing RING closure */
		      z = 0.0;
		      m = 0.0;
		      if (i_ring->DimensionModel == GAIA_XY_Z)
			{
			    gaiaGetPointXYZ (i_ring->Coords, 0, &x, &y, &z);
			}
		      else if (i_ring->DimensionModel == GAIA_XY_M)
			{
			    gaiaGetPointXYM (i_ring->Coords, 0, &x, &y, &m);
			}
		      else if (i_ring->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaGetPointXYZM (i_ring->Coords, 0, &x, &y, &z,
					      &m);
			}
		      else
			{
			    gaiaGetPoint (i_ring->Coords, 0, &x, &y);
			}
		      points = o_ring->Points - 1;
		      if (o_ring->DimensionModel == GAIA_XY_Z)
			{
			    gaiaSetPointXYZ (o_ring->Coords, points, x, y, z);
			}
		      else if (o_ring->DimensionModel == GAIA_XY_M)
			{
			    gaiaSetPointXYM (o_ring->Coords, points, x, y, m);
			}
		      else if (o_ring->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaSetPointXYZM (o_ring->Coords, points, x, y, z,
					      m);
			}
		      else
			{
			    gaiaSetPoint (o_ring->Coords, points, x, y);
			}
		  }
	    }
	  polyg = polyg->Next;
      }
    return new_geom;
}

GAIAGEO_DECLARE int
gaiaIsToxic (gaiaGeomCollPtr geom)
{
/* 
/ identifying toxic geometries 
/ i.e. geoms making GEOS to crash !!!
*/
    int ib;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    gaiaRingPtr ring;
    if (!geom)
	return 0;
    point = geom->FirstPoint;
    while (point)
      {
	  /* checking POINTs */
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* checking LINESTRINGs */
	  if (line->Points < 2)
	      return 1;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* checking POLYGONs */
	  ring = polyg->Exterior;
	  if (ring->Points < 4)
	      return 1;
	  for (ib = 0; ib < polyg->NumInteriors; ib++)
	    {
		ring = polyg->Interiors + ib;
		if (ring->Points < 4)
		    return 1;
	    }
	  polyg = polyg->Next;
      }
    return 0;
}
