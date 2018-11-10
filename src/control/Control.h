/*
 * Xournal++
 *
 * The main Control
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include "Actions.h"
#include "ClipboardHandler.h"
#include "LatexAction.h"
#include "RecentManager.h"
#include "ScrollHandler.h"
#include "ToolHandler.h"
#include "ZoomControl.h"

#include "gui/dialog/LatexGlade.h"
#include "gui/MainWindow.h"
#include "gui/SearchBar.h"
#include "gui/sidebar/Sidebar.h"
#include "jobs/ProgressListener.h"
#include "jobs/XournalScheduler.h"
#include "model/Document.h"
#include "settings/Settings.h"
#include "undo/UndoRedoHandler.h"

#include <XournalType.h>

#include <vector>

class Sidebar;
class CallbackData;
class PageView;
class SaveHandler;
class GladeSearchpath;
class MetadataManager;
class Cursor;
class ToolbarDragDropHandler;

class Control : public ActionHandler,
	public ToolListener,
	public DocumentHandler,
	public RecentManagerListener,
	public UndoRedoListener,
	public ClipboardListener,
	public ProgressListener
{
public:
	Control(GladeSearchpath* gladeSearchPath, bool noThreads = false);
	virtual ~Control();

	void initWindow(MainWindow* win);
public:
	// Menu File
	bool newFile();
	bool openFile(path filename = "", int scrollToPage = -1);
	bool annotatePdf(path filename, bool attachPdf, bool attachToDocument);
	void print();
	void exportAsPdf();
	void exportAs();
	bool save(bool synchron = false);
	bool saveAs();
	void quit();
	bool close(bool destroy = false);

	void resetShapeRecognizer();

	// Menu edit
	void showSettings();

	// Menu tools
	void runLatex();

	// Menu Help
	void showAbout();

	virtual void actionPerformed(ActionType type, ActionGroup group, GdkEvent* event, GtkMenuItem* menuitem,
								 GtkToolButton* toolbutton, bool enabled);

	virtual void toolColorChanged();
	virtual void setCustomColorSelected();
	virtual void toolChanged();
	virtual void toolSizeChanged();

	void selectTool(ToolType type);
	void selectDefaultTool();

	void updatePageNumbers(size_t page, size_t pdfPage);

	virtual void fileOpened(const char* uri);

	/**
	 * Save current state (selected tool etc.)
	 */
	void saveSettings();

	void updateWindowTitle();
	void calcZoomFitSize();
	void setViewTwoPages(bool continous);
	void setViewPresentationMode(bool continous);
	void setPageInsertType(PageInsertType type);
	void manageToolbars();
	void customizeToolbars();
	void enableFullscreen(bool enabled, bool presentation = false);

	void gotoPage();

	void setRulerEnabled(bool enabled);
	void setRectangleEnabled(bool enabled);
	void setCircleEnabled(bool enabled);
	void setArrowEnabled(bool enabled);
	void setShapeRecognizerEnabled(bool enabled);

	void addNewLayer();
	void deleteCurrentLayer();
	void switchToLay(int layer);

	void paperFormat();
	void changePageBackgroundColor();
	void setPageBackground(ActionType type);
	void updateBackgroundSizeButton();

	void endDragDropToolbar();
	void startDragDropToolbar();
	bool isInDragAndDropToolbar();

	bool isFullscreen();

	bool searchTextOnPage(string text, int p, int* occures, double* top);

	/**
	 * Fire page selected, but first check if the page Number is valid
	 *
	 * @return the page ID or size_t_npos if the page is not found
	 */
	size_t firePageSelected(PageRef page);
	void firePageSelected(size_t page);

	void addDefaultPage();
	void insertNewPage(size_t position);
	void insertPage(PageRef page, size_t position);
	void deletePage();

	/**
	 * Disable / enable delete page button
	 */
	void updateDeletePageButton();

	// selection handling
	void clearSelection();

	void setCopyPasteEnabled(bool enabled);

	void enableAutosave(bool enable);

	void getDefaultPagesize(double& width, double& height);

	void clearSelectionEndText();

	void setToolSize(ToolSize size);

	TextEditor* getTextEditor();

	GladeSearchpath* getGladeSearchPath();

	void disableSidebarTmp(bool disabled);

	XournalScheduler* getScheduler();

	void block(string name);
	void unblock();

	void renameLastAutosaveFile();
	void setLastAutosaveFile(path newAutosaveFile);
	void deleteLastAutosaveFile(path newAutosaveFile);
	void setClipboardHandlerSelection(EditSelection* selection);

	MetadataManager* getMetadataManager();
	Settings* getSettings();
	ToolHandler* getToolHandler();
	ZoomControl* getZoomControl();
	Document* getDocument();
	UndoRedoHandler* getUndoRedoHandler();
	MainWindow* getWindow();
	RecentManager* getRecentManager();
	ScrollHandler* getScrollHandler();
	PageRef getCurrentPage();
	size_t getCurrentPageNo();
	Cursor* getCursor();
	Sidebar* getSidebar();
	SearchBar* getSearchBar();

	bool copy();
	bool cut();
	bool paste();

	void help();

public:
	// UndoRedoListener interface
	void undoRedoChanged();
	void undoRedoPageChanged(PageRef page);

public:
	// ProgressListener interface
	void setMaximumState(int max);
	void setCurrentState(int state);

public:
	// ClipboardListener interface
	virtual void clipboardCutCopyEnabled(bool enabled);
	virtual void clipboardPasteEnabled(bool enabled);
	virtual void clipboardPasteText(string text);
	virtual void clipboardPasteImage(GdkPixbuf* img);
    virtual void clipboardPasteTex(GdkPixbuf* img, const char* text, int textLength);
	virtual void clipboardPasteXournal(ObjectInputStream& in);
	virtual void deleteSelection();

	void clipboardPaste(Element* e);

protected:
	static bool invokeCallback(CallbackData* cb);
	void invokeLater(ActionType type);
	void zoomFit();

	bool showSaveDialog();

	void fileLoaded(int scrollToPage = -1);

	void eraserSizeChanged();
	void penSizeChanged();
	void hilighterSizeChanged();

	static bool checkChangedDocument(Control* control);
	static bool autosaveCallback(Control* control);

	void fontChanged();

private:
	XOJ_TYPE_ATTRIB;


	RecentManager* recent;
	UndoRedoHandler* undoRedo;
	ZoomControl* zoom;
	bool fullscreen;

	Settings* settings;
	MainWindow* win;

	Document* doc;

	Sidebar* sidebar;
	SearchBar* searchBar;

	ToolHandler* toolHandler;

	ActionType lastAction;
	ActionGroup lastGroup;
	bool lastEnabled;

	GList* hiddenFullscreenWidgets;
	bool sidebarHidden;

	ScrollHandler* scrollHandler;

	ToolbarDragDropHandler* dragDropHandler;

	/**
	 * The cursor handler
	 */
	Cursor* cursor;

	/**
	 * Timeout id: the timeout watches the changes and actualizes the previews from time to time
	 */
	int changeTimout;

	/**
	 * The pages wihch has changed since the last update (for preview update)
	 */
	std::vector<XojPage*> changedPages;

	/**
	 * Our clipboard abstraction
	 */
	ClipboardHandler* clipboardHandler;

	/**
	 * The autosave handler ID
	 */
	int autosaveTimeout;
	path lastAutosaveFilename;

	/**
	 * Default page size
	 */
	double defaultWidth;
	double defaultHeight;

	XournalScheduler* scheduler;

	/**
	 * State / Blocking attributes
	 */
	GtkWidget* statusbar;
	GtkLabel* lbState;
	GtkProgressBar* pgState;
	int maxState;
	bool isBlocking;

	GladeSearchpath* gladeSearchPath;

	MetadataManager* metadata;

};

class CallbackData
{
public:
	CallbackData(Control* control, ActionType type)
	{
		this->control = control;
		this->type = type;
	}

	ActionType type;
	Control* control;
};
