// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_GUI_FILE_OPEN_DIALOG_H_INCLUDED__
#define __C_GUI_FILE_OPEN_DIALOG_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIFileOpenDialog.h"
#include "IGUIButton.h"
#include "IGUIListBox.h"
#include "IGUIEditBox.h"
#include "IFileSystem.h"

namespace irr
{
namespace gui
{

	class CGUIFileOpenDialog : public IGUIFileOpenDialog
	{
	public:

		//! constructor
		CGUIFileOpenDialog(const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, s32 id);

		//! destructor
		virtual ~CGUIFileOpenDialog() _IRR_OVERRIDE_;

		//! returns the filename of the selected file. Returns NULL, if no file was selected.
		virtual const wchar_t* getFileName() const _IRR_OVERRIDE_;

		//! Returns the directory of the selected file. Returns NULL, if no directory was selected.
		virtual const io::path& getDirectoryName() _IRR_OVERRIDE_;

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event) _IRR_OVERRIDE_;

		//! draws the element and its children
		virtual void draw() _IRR_OVERRIDE_;

	protected:

		//! fills the listbox with files.
		void fillListBox();

		//! sends the event that the file has been selected.
		void sendSelectedEvent( EGUI_EVENT_TYPE type );

		//! sends the event that the file choose process has been canceld
		void sendCancelEvent();

		core::position2d<s32> DragStart;
		core::stringw FileName;
		io::path FileDirectory;

		IGUIButton* CloseButton;
		IGUIButton* OKButton;
		IGUIButton* CancelButton;
		IGUIListBox* FileBox;
		IGUIEditBox* FileNameText;
		IGUIElement* EventParent;
		io::IFileSystem* FileSystem;
		io::IFileList* FileList;
		bool Dragging;
	};


} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

#endif // __C_GUI_FILE_OPEN_DIALOG_H_INCLUDED__

