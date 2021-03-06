//-----------------------------------------------------------------------------
// Copyright (c) 2015 Andrew Mac
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

#include <wx/dir.h>
#include <wx/treectrl.h>

// UI generated from wxFormBuilder
#include "Torque6EditorUI.h"

#include "profilerWindow.h"

ProfilerWindow::ProfilerWindow(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager)
   : Parent(_EditorManager, _frame, _manager),
     mProfilerPanel( NULL )
{
   mFrameCount = 50;
}

ProfilerWindow::~ProfilerWindow()
{

}

void ProfilerWindow::initWindow()
{
   // Create panel.
   mProfilerPanel = new ProfilerPanel(mFrame, wxID_ANY);

   // Events
   mProfilerPanel->Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ProfilerWindow::OnButtonEvent), NULL, this);

   // Add Pane
   mManager->AddPane(mProfilerPanel, wxAuiPaneInfo().Caption("Profiler")
                                            .CaptionVisible( true )
                                            .CloseButton( true )
                                            .PinButton( true )
                                            .MaximizeButton(true)
                                            .Dock()
                                            .Resizable()
                                            .FloatingSize( wxDefaultSize )
                                            .Bottom()
                                            .Hide());
   mManager->Update();

   mProfilerPanel->ProfilerDataView->AssociateModel( &mProfilerData );

   // Column 0
   wxDataViewTextRenderer *tr = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );
   wxDataViewColumn* column0 = new wxDataViewColumn( "Name", tr, 0, 400, wxALIGN_LEFT, 
                                                     wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE );
   mProfilerPanel->ProfilerDataView->AppendColumn( column0 );

   // Column 1
   tr = new wxDataViewTextRenderer( "long", wxDATAVIEW_CELL_INERT );
   wxDataViewColumn *column1 = new wxDataViewColumn( "Count", tr, 1, 100, wxALIGN_LEFT,
                                                      wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE );
   mProfilerPanel->ProfilerDataView->AppendColumn( column1 );

   // Column 2
   tr = new wxDataViewTextRenderer( "double", wxDATAVIEW_CELL_INERT );
   wxDataViewColumn *column2 = new wxDataViewColumn( "Time", tr, 2, 100, wxALIGN_LEFT,
                                                      wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE );
   mProfilerPanel->ProfilerDataView->AppendColumn( column2 );

   // Column 3
   mProfilerPanel->ProfilerDataView->AppendProgressColumn( "Percent", 3, wxDATAVIEW_CELL_INERT, 100 );
}

void ProfilerWindow::openWindow()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mProfilerPanel);
   paneInfo.Show();
   mManager->Update();
}

void ProfilerWindow::closeWindow()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mProfilerPanel);
   paneInfo.Hide();
   mManager->Update();
}

void ProfilerWindow::processProfilerCachedData(ProfilerCachedData* data, ProfilerTreeModelNode* node)
{
   wxString node_name(data->name);
   if ( strcmp(data->name, "ROOT") == 0 )
      node_name = wxNow();

   ProfilerTreeModelNode* child_node = mProfilerData.AddEntry(node_name, data->count / mFrameCount, data->time / (F64)mFrameCount, data->percent, node);

   for(int i = 0; i < data->children.size(); ++i )
      processProfilerCachedData(&data->children[i], child_node);
}

void ProfilerWindow::OnButtonEvent( wxCommandEvent& evt )
{
   if ( evt.GetId() == PROFILER_START )
   {
      Torque::Engine.ProfilerLink->enable(true);

      // Run Main Loop
      for(S32 i = 0; i < mFrameCount; i++)
         Torque::Engine.mainLoop();

      Torque::Engine.ProfilerLink->enable(false);
      Torque::Engine.ProfilerLink->dumpToCache();
      Torque::Engine.mainLoop();

      ProfilerCachedData* data = Torque::Engine.ProfilerLink->getCachedData();
      processProfilerCachedData(&data->children[0], NULL);
   }
}

void ProfilerWindow::onProjectLoaded(const wxString& projectName, const wxString& projectPath)
{
   if ( !mOpen ) return;
}

void ProfilerWindow::onProjectClosed()
{
   if ( !mOpen ) return;
}