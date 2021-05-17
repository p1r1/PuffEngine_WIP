/***********************************************************************
    created:    Mon Jan 10 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************

This file can be used to set certain configuration options which are used
when compiling the samples for Crazy Eddie's GUI System.

Each item in here has a comment to describe what it's for.

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

#ifndef _CEGUISamplesConfig_h_
#define _CEGUISamplesConfig_h_

// Bring in main config file
#include "CEGUI/Config.h"

/* #undef CEGUI_SAMPLE_DATAPATH */
#ifndef CEGUI_SAMPLE_DATAPATH

// Define the default place where samples will look for datafiles
#if defined(_WIN32) || defined(__WIN32__)
#   define CEGUI_SAMPLE_DATAPATH "C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-0.8.7/datafiles"
#else
#   define CEGUI_SAMPLE_DATAPATH "C:/Program Files (x86)/cegui/share/cegui-0"
#endif

#endif

//////////////////////////////////////////////////////////////////////////
// Comment/Uncomment these lines to control renderer use in samples.
//////////////////////////////////////////////////////////////////////////
#define CEGUI_BUILD_RENDERER_OPENGL
#define CEGUI_BUILD_RENDERER_OPENGL3
/* #undef CEGUI_SAMPLES_SUPPORT_RENDERER_OPENGLES2 */
#define CEGUI_USE_GLFW_VER 2
/* #undef CEGUI_BUILD_RENDERER_OGRE */
/* #undef CEGUI_BUILD_RENDERER_IRRLICHT */
/* #undef CEGUI_OPENGL_VER_MAJOR_FORCE */
#define CEGUI_OPENGL_VER_MINOR_FORCE 
/* #undef CEGUI_BUILD_RENDERER_DIRECT3D9 */
/* #undef CEGUI_BUILD_RENDERER_DIRECT3D10 */
/* #undef CEGUI_BUILD_RENDERER_DIRECT3D11 */
/* #undef CEGUI_BUILD_RENDERER_DIRECTFB */
/* #undef CEGUI_SAMPLES_USE_GTK2 */

#define CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE
/* #undef CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE */
/* #undef CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE */
/* #undef CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE */
/* #undef CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE */
/* #undef CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE */
/* #undef CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE */

#endif  // end of guard _CEGUISamplesConfig_h_

