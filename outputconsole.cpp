/*
  Copyright (C) 2012 - Voidious

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "outputconsole.h"

#include <wx/wx.h>

OutputConsole::OutputConsole(wxWindowID id, const char *title)
    : wxFrame(NULL, id, title, wxPoint(50, 50), wxSize(600, 450),
              wxDEFAULT_FRAME_STYLE) {
  output_ = new wxTextCtrl(this, id + 1, "", wxPoint(0, 0), wxSize(400, 350),
                          wxTE_MULTILINE | wxTE_READONLY, wxDefaultValidator);
  output_->SetFont(wxFont(12, wxFONTFAMILY_TELETYPE));
  Connect(id, wxEVT_CLOSE_WINDOW,
          wxCommandEventHandler(OutputConsole::onClose));
  eventFilter_ = new OutputConsoleEventFilter(this);
  this->GetEventHandler()->AddFilter(eventFilter_);
}

OutputConsole::~OutputConsole() {
  this->GetEventHandler()->RemoveFilter(eventFilter_);
  delete eventFilter_;
  delete output_;
}

void OutputConsole::print(const char *text) {
  output_->WriteText(text);
}

void OutputConsole::println(const char *text) {
  output_->WriteText(text);
  output_->WriteText("\n");
}

void OutputConsole::println() {
  output_->WriteText("\n");
}

void OutputConsole::clear() {
  output_->Clear();
}

void OutputConsole::onClose(wxCommandEvent &event) {
  Hide();
}

OutputConsoleEventFilter::OutputConsoleEventFilter(
    OutputConsole *outputConsole) {
  outputConsole_ = outputConsole;
}

OutputConsoleEventFilter::~OutputConsoleEventFilter() {
  
}

int OutputConsoleEventFilter::FilterEvent(wxEvent& event) {
  const wxEventType type = event.GetEventType();
  wxKeyEvent *keyEvent = ((wxKeyEvent*) &event);
  int keyCode = keyEvent->GetKeyCode();
  if (type == wxEVT_KEY_DOWN && outputConsole_->IsActive()) {
    if (keyCode == WXK_ESCAPE) {
      outputConsole_->Hide();
    }
#ifdef __WXOSX__
    if (keyEvent->GetUnicodeKey() == 'W' && keyEvent->ControlDown()) {
      outputConsole_->Hide();
    }
#endif
  }
  return Event_Skip;
}
