//-----------------------------------------------------------------------------
// Copyright (c) 2016 Andrew Mac
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "editorTool.h"

wxVector<EditorTool*> EditorTool::smEditorTools;

EditorTool::EditorTool(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager)
   :  mActive(false),
      mEditorManager(_EditorManager),
      mFrame(_frame),
      mManager(_manager)
{
   //
}

void EditorTool::activateTool(EditorTool* tool, S32 index)
{
   for (S32 n = 0; n < smEditorTools.size(); ++n)
   {
      if (tool == smEditorTools[n])
         tool->onActivateTool(index);
      else if (tool->mActive)
         tool->onDeactivateTool();
   }
}

template <typename T>
static T* EditorTool::findTool()
{
   for (S32 n = 0; n < smEditorTools.size(); ++n)
   {
      T* result = dynamic_cast<T*>(smEditorTools[n]);
      if (result)
         return result;
   }

   return NULL;
}