/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.

 */

/**
 @file WareSrcWidget.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QList>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>
#include <openfluid/ui/waresdev/WareSrcFileEditor.hpp>
#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/TextEditMsgStream.hpp>
#include <openfluid/ui/waresdev/NewSrcFileAssistant.hpp>
#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>
#include <openfluid/ui/waresdev/WareshubJsonEditor.hpp>

#include "ui_WareSrcWidget.h"


namespace openfluid { namespace ui { namespace waresdev {


WareSrcWidget::WareSrcWidget(const openfluid::waresdev::WareSrcManager::PathInfo& Info, bool IsStandalone,
                             openfluid::waresdev::WareSrcContainer::ConfigMode Config,
                             openfluid::waresdev::WareSrcContainer::BuildMode Build, QWidget* Parent) :
    QWidget(Parent), ui(new Ui::WareSrcWidget),
    m_Container(Info.m_AbsolutePathOfWare, Info.m_WareType, Info.m_WareName), m_IsStandalone(IsStandalone)
{
  ui->setupUi(this);

  QList<int> Sizes;
  Sizes << 1000 << 180;
  ui->splitter->setSizes(Sizes);

  if (m_IsStandalone)
  {
    mp_StandaloneToolBar = new WareSrcToolbar(true, this);

    ui->Toolbar_Layout->addWidget(mp_StandaloneToolBar);

    connect(mp_StandaloneToolBar->action("NewFile"), SIGNAL(triggered()), this, SLOT(newFile()));
    connect(mp_StandaloneToolBar->action("OpenFile"), SIGNAL(triggered()), this, SLOT(openFile()));
    connect(mp_StandaloneToolBar->action("SaveFile"), SIGNAL(triggered()), this, SLOT(saveCurrentEditor()));
    connect(mp_StandaloneToolBar->action("SaveAsFile"), SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(mp_StandaloneToolBar->action("SaveAllFiles"), SIGNAL(triggered()), this, SLOT(saveAllFileTabs()));
    connect(mp_StandaloneToolBar->action("CloseFile"), SIGNAL(triggered()), this, SLOT(closeCurrentEditor()));
    connect(mp_StandaloneToolBar->action("DeleteFile"), SIGNAL(triggered()), this, SLOT(deleteCurrentFile()));

    connect(mp_StandaloneToolBar->action("Copy"), SIGNAL(triggered()), this, SLOT(copyText()));
    connect(mp_StandaloneToolBar->action("Cut"), SIGNAL(triggered()), this, SLOT(cutText()));
    connect(mp_StandaloneToolBar->action("Paste"), SIGNAL(triggered()), this, SLOT(pasteText()));
    connect(mp_StandaloneToolBar->action("FindReplace"), SIGNAL(triggered()), this, SIGNAL(findReplaceRequested()));
    connect(mp_StandaloneToolBar->action("GoToLine"), SIGNAL(triggered()), this, SLOT(goToLine()));

    connect(mp_StandaloneToolBar->action("Release"), SIGNAL(triggered()), this, SLOT(setReleaseMode()));
    connect(mp_StandaloneToolBar->action("Debug"), SIGNAL(triggered()), this, SLOT(setDebugMode()));
    connect(mp_StandaloneToolBar->action("BuildInstall"), SIGNAL(triggered()), this, SLOT(setBuildWithInstallMode()));
    connect(mp_StandaloneToolBar->action("BuildOnly"), SIGNAL(triggered()), this, SLOT(setBuildNoInstallMode()));
    connect(mp_StandaloneToolBar->action("Configure"), SIGNAL(triggered()), this, SLOT(configure()));
    connect(mp_StandaloneToolBar->action("Build"), SIGNAL(triggered()), this, SLOT(build()));

    connect(mp_StandaloneToolBar->action("OpenTerminal"), SIGNAL(triggered()), this, SIGNAL(openTerminalRequested()));
    connect(mp_StandaloneToolBar->action("OpenExplorer"), SIGNAL(triggered()), this, SIGNAL(openExplorerRequested()));

    connect(mp_StandaloneToolBar->action("APIDoc"), SIGNAL(triggered()), this, SIGNAL(openAPIDocRequested()));
  }

  connect(ui->WareSrcFileCollection, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseFileTabRequested(int)));
  connect(ui->WareSrcFileCollection, SIGNAL(currentChanged(int)), this, SLOT(onCurrentTabChanged(int)));

  connect(&m_Container, SIGNAL(processFinished()), this, SLOT(onProcessFinished()));

  connect(&m_Container, SIGNAL(configureProcessLaunched(openfluid::ware::WareType, const QString&)),
          this, SLOT(notifyConfigureLaunched(openfluid::ware::WareType, const QString&)));

  connect(&m_Container, SIGNAL(configureProcessFinished(openfluid::ware::WareType, const QString&)),
          this, SLOT(notifyConfigureFinished(openfluid::ware::WareType, const QString&)));

  connect(&m_Container, SIGNAL(buildProcessLaunched(openfluid::ware::WareType, const QString&)),
          this, SLOT(notifyBuildLaunched(openfluid::ware::WareType, const QString&)));

  connect(&m_Container, SIGNAL(buildProcessFinished(openfluid::ware::WareType, const QString&)),
          this, SLOT(notifyBuildFinished(openfluid::ware::WareType, const QString&)));


  m_Container.setConfigMode(Config);
  m_Container.setBuildMode(Build);

  mp_TextEditMsgStream = new openfluid::ui::waresdev::TextEditMsgStream(ui->WareSrcMessageWidget);
  m_Container.setMsgStream(*mp_TextEditMsgStream);

  connect(ui->WareSrcMessageWidget, SIGNAL(messageClicked(openfluid::waresdev::WareSrcMsgParser::WareSrcMsg&)), this,
          SLOT(onMessageClicked(openfluid::waresdev::WareSrcMsgParser::WareSrcMsg&)));
}


// =====================================================================
// =====================================================================


WareSrcWidget::~WareSrcWidget()
{
  delete mp_TextEditMsgStream;
  delete ui;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::openFileTab(const openfluid::waresdev::WareSrcManager::PathInfo& Info, int Index)
{
  if (!setCurrent(Info))
    addNewFileTab(Index, Info.m_AbsolutePath, Info.m_FileName, QDir::toNativeSeparators(Info.m_RelativePathToWareDir));
}


// =====================================================================
// =====================================================================


void WareSrcWidget::addNewFileTab(int Index, const QString& AbsolutePath, const QString& TabLabel,
                                  const QString& TabTooltip)
{
  WareFileEditor* Editor;

  if (QFileInfo(AbsolutePath).fileName() == "wareshub.json")
  {
    try
    {
      WareshubJsonEditor* JsonEditor = new WareshubJsonEditor(AbsolutePath, this);

      connect(JsonEditor, SIGNAL(editorChanged(WareFileEditor*,bool)), this,
              SLOT(onEditorTxtModified(WareFileEditor*,bool)));

      connect(JsonEditor, SIGNAL(editorSaved()), this, SIGNAL(editorSaved()));

      Editor = JsonEditor;
    }
    catch (std::exception& e)
    {
      QMessageBox::critical(this, tr("File error"), QString::fromUtf8(e.what()));
      return;
    }
  }
  else
  {
    WareSrcFileEditor* SrcEditor = new WareSrcFileEditor(AbsolutePath, this);

    for (openfluid::waresdev::WareSrcMsgParser::WareSrcMsg Msg : m_Container.getMessages())
    {
      if (Msg.m_Path == AbsolutePath)
        SrcEditor->addLineMessage(Msg);
    }

    connect(SrcEditor, SIGNAL(editorTxtChanged(WareFileEditor*,bool)), this,
            SLOT(onEditorTxtModified(WareFileEditor*,bool)));

    connect(SrcEditor, SIGNAL(editorSaved()), this, SIGNAL(editorSaved()));

    Editor = SrcEditor;
  }

  QWidget* Widget = Editor->getWidget();

  int Pos = ui->WareSrcFileCollection->insertTab(Index, Widget, TabLabel);
  ui->WareSrcFileCollection->setTabToolTip(Pos, TabTooltip);

  m_WareFilesByPath[AbsolutePath] = Editor;

  ui->WareSrcFileCollection->setCurrentWidget(Widget);

  if (m_IsStandalone)
    Widget->installEventFilter(this);
}


// =====================================================================
// =====================================================================


int WareSrcWidget::closeCurrentEditor(bool WithConfirm)
{
  return onCloseFileTabRequested(ui->WareSrcFileCollection->currentIndex(), WithConfirm);
}


// =====================================================================
// =====================================================================


int WareSrcWidget::onCloseFileTabRequested(int Index, bool WithConfirm)
{
  int ClosedTabPos = -1;

  if (WareFileEditor* Editor = dynamic_cast<WareFileEditor*>(ui->WareSrcFileCollection->widget(Index)))
  {
    int Choice = QMessageBox::Discard;

    if (WithConfirm && Editor->isModified())
    {
      QMessageBox MsgBox;
      MsgBox.setText(tr("The document has been modified."));
      MsgBox.setInformativeText(tr("Do you want to save changes?"));
      MsgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      MsgBox.setDefaultButton(QMessageBox::Save);
      Choice = MsgBox.exec();
    }

    switch (Choice)
    {
      case QMessageBox::Save:
        Editor->saveContent();
      case QMessageBox::Discard:
        ClosedTabPos = closeFileTab(Editor);
        break;
      case QMessageBox::Cancel:
      default:
        break;
    }

  }

  return ClosedTabPos;
}


// =====================================================================
// =====================================================================


int WareSrcWidget::closeFileTab(WareFileEditor* Editor)
{
  if (!Editor)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Null Editor");

  int Index = ui->WareSrcFileCollection->indexOf(Editor->getWidget());

  ui->WareSrcFileCollection->removeTab(Index);

  m_WareFilesByPath.remove(Editor->getFilePath());

  delete Editor;

  emit wareTextModified(this, isWareModified());

  checkModifiedStatus();

  return Index;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::saveAllFileTabs()
{
  for(WareFileEditor* Editor : m_WareFilesByPath.values())
    Editor->saveContent();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::closeAllFileTabs()
{
  for(WareFileEditor* Editor : m_WareFilesByPath.values())
    closeFileTab(Editor);
}


// =====================================================================
// =====================================================================


QString WareSrcWidget::getCurrentFilePath()
{
  if (WareFileEditor* Editor = currentEditor())
    return Editor->getFilePath();

  return "";
}


// =====================================================================
// =====================================================================


WareFileEditor* WareSrcWidget::currentEditor()
{
  return dynamic_cast<WareFileEditor*>(ui->WareSrcFileCollection->currentWidget());
}


// =====================================================================
// =====================================================================


int WareSrcWidget::closeFileTab(const QString& Path)
{
  if (m_WareFilesByPath.contains(Path))
    return closeFileTab(m_WareFilesByPath.value(Path));

  return -1;
}


// =====================================================================
// =====================================================================


bool WareSrcWidget::eventFilter(QObject* Obj, QEvent* Event)
{
  if (Event->type() == QEvent::ShortcutOverride && static_cast<QKeyEvent*>(Event)->matches(QKeySequence::Save))
  {
    saveCurrentEditor();
    Event->accept();
    return true;
  }

  return QObject::eventFilter(Obj, Event);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::openDefaultFiles()
{
  for(const QString& F : m_Container.getDefaultFilesPaths())
  {
    QString FileName = QFileInfo(F).fileName();
    addNewFileTab(-1,F, FileName, QDir::toNativeSeparators(FileName));
  }
}


// =====================================================================
// =====================================================================


bool WareSrcWidget::setCurrent(const openfluid::waresdev::WareSrcManager::PathInfo& Info)
{
  WareFileEditor* Widget = m_WareFilesByPath.value(Info.m_AbsolutePath, 0);

  if (Widget)
    ui->WareSrcFileCollection->setCurrentWidget(Widget->getWidget());

  return Widget;
}


// =====================================================================
// =====================================================================


openfluid::waresdev::WareSrcContainer& WareSrcWidget::wareSrcContainer()
{
  return m_Container;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setReleaseMode()
{
  m_Container.setConfigMode(openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_RELEASE);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setDebugMode()

{
  m_Container.setConfigMode(openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_DEBUG);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setBuildWithInstallMode()

{
  m_Container.setBuildMode(openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_WITHINSTALL);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setBuildNoInstallMode()

{
  m_Container.setBuildMode(openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_NOINSTALL);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::configure()

{
  clearEditorsMessages();

  try
  {
    m_Container.configure();
  }
  catch (openfluid::base::FrameworkException& E )
  {
    QMessageBox::critical(nullptr, tr("Configure error"), QString::fromStdString(E.getMessage()));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::build()

{
  clearEditorsMessages();

  try
  {
    m_Container.build();
  }
  catch (openfluid::base::FrameworkException& E )
  {
    QMessageBox::critical(nullptr, tr("Build error"), QString::fromStdString(E.getMessage()));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onEditorTxtModified(WareFileEditor* Editor, bool Modified)
{
  int Pos = ui->WareSrcFileCollection->indexOf(Editor->getWidget());

  if (Pos > -1)
  {
    QString Label = ui->WareSrcFileCollection->tabText(Pos);
    bool ModifiedState = Label.startsWith("*");

    if (Modified && !ModifiedState)
      ui->WareSrcFileCollection->setTabText(Pos, Label.prepend("*"));
    else if (!Modified && ModifiedState)
      ui->WareSrcFileCollection->setTabText(Pos, Label.remove(0, 1));

    emit wareTextModified(this, isWareModified());

    checkModifiedStatus();
  }

}


// =====================================================================
// =====================================================================


bool WareSrcWidget::isWareModified()
{
  for(WareFileEditor* Editor : m_WareFilesByPath.values())
  {
    if(Editor->isModified())
      return true;
  }

  return false;
}



// =====================================================================
// =====================================================================


bool WareSrcWidget::isWareProcessRunning() const
{
  return m_Container.isProcessRunning();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::saveCurrentEditor()
{
  if (WareFileEditor* Editor = currentEditor())
    Editor->saveContent();
}


// =====================================================================
// =====================================================================


QString WareSrcWidget::saveAs(const QString& TopDirectory)
{
  WareFileEditor* CurrentEditor = currentEditor();
  if (!CurrentEditor)
  {
    QMessageBox::warning(0, tr("No open file"), tr("No file to save"));
    return "";
  }

  QString CurrentFilePath = CurrentEditor->getFilePath();
  QString CurrentWarePath = m_Container.getAbsolutePath();

  QString NewFilePath = WareExplorerDialog::getSaveFilePath(this,
                                                            TopDirectory.isEmpty() ? CurrentWarePath : TopDirectory,
                                                            CurrentFilePath);

  if (NewFilePath.isEmpty())
    return "";

  if (NewFilePath == CurrentFilePath)
  {
    saveCurrentEditor();
    return "";
  }

  CurrentEditor->saveContentToPath(NewFilePath);

  openfluid::waresdev::WareSrcManager::PathInfo NewPathInfo = openfluid::waresdev::WareSrcManager::instance()
      ->getPathInfo(NewFilePath);

  int CurrentIndex = closeFileTab(CurrentEditor);

  if (NewPathInfo.m_AbsolutePathOfWare == CurrentWarePath)
  {
    // in case the new path was already opened
    closeFileTab(NewFilePath);

    openFileTab(NewPathInfo, CurrentIndex);

    return "";
  }

  return NewFilePath;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::newFile()
{
  NewSrcFileAssistant Assistant(m_Container, this);
  if(Assistant.exec() == QDialog::Accepted)
  {
    QString NewFilePath = Assistant.getNewFilePath();
    if (!NewFilePath.isEmpty())
    {
      m_Container.update();
      openFileTab(openfluid::waresdev::WareSrcManager::instance()->getPathInfo(NewFilePath));
    }
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::deleteCurrentFile()
{
  if (WareFileEditor* Editor = currentEditor())
  {
    QString Path = Editor->getFilePath();

    if (QMessageBox::warning(this, tr("Delete file"), tr("Are you sure you want to delete \"%1\"?").arg(Path),
                             QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel)
        == QMessageBox::Cancel)
      return;

    // Careful as it deletes Editor
    closeFileTab(Editor);

    if (QDir().remove(Path))
      m_Container.update();
    else
      QMessageBox::critical(0, "Error", tr("Unable to remove the file \"%1\"").arg(Path));
  }
  else
    QMessageBox::warning(0, tr("No open file"), tr("No file to delete"));
}


// =====================================================================
// =====================================================================


void WareSrcWidget::openFile()
{
  QString PathToOpen = openfluid::ui::waresdev::WareExplorerDialog::getOpenFilePath(this, m_Container.getAbsolutePath(),
                                                                                    getCurrentFilePath());

  if (PathToOpen.isEmpty())
    return;

  openFileTab(openfluid::waresdev::WareSrcManager::instance()->getPathInfo(PathToOpen));
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onCurrentTabChanged(int Index)
{
  if (WareFileEditor* Editor = dynamic_cast<WareFileEditor*>(ui->WareSrcFileCollection->widget(Index)))
    emit currentTabChanged(Editor->getFilePath());

  checkModifiedStatus();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::copyText()
{
  if (WareFileEditor* Editor = currentEditor())
    Editor->copy();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::cutText()
{
  if (WareFileEditor* Editor = currentEditor())
    Editor->cut();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::pasteText()
{
  if (WareFileEditor* Editor = currentEditor())
    Editor->paste();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::clearEditorsMessages()
{
  for (WareFileEditor* Editor : m_WareFilesByPath)
    Editor->clearLineMessages();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onProcessFinished()
{
  for (openfluid::waresdev::WareSrcMsgParser::WareSrcMsg Msg : m_Container.getMessages())
  {
    if (WareFileEditor* Editor = m_WareFilesByPath.value(Msg.m_Path, 0))
      Editor->addLineMessage(Msg);
  }

  for (WareFileEditor* Editor : m_WareFilesByPath)
    Editor->updateLineNumberArea();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onMessageClicked(openfluid::waresdev::WareSrcMsgParser::WareSrcMsg& Msg)
{
  openfluid::waresdev::WareSrcManager::PathInfo Info = openfluid::waresdev::WareSrcManager::instance()->getPathInfo(
      Msg.m_Path);

  if (Info.m_AbsolutePathOfWare != m_Container.getAbsolutePath())
    return;

  openFileTab(Info);

  if (WareFileEditor* Editor = m_WareFilesByPath.value(Msg.m_Path, 0))
    Editor->selectLine(Msg.m_LineNb);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::checkModifiedStatus()
{
  bool IsCurrentEditorModified = false;
  if (WareFileEditor* Editor = currentEditor())
    IsCurrentEditorModified = Editor->isModified();

  bool IsWareModified = isWareModified();

  if (mp_StandaloneToolBar)
  {
    mp_StandaloneToolBar->action("SaveFile")->setEnabled(IsCurrentEditorModified);
    mp_StandaloneToolBar->action("SaveAsFile")->setEnabled(IsCurrentEditorModified);
    mp_StandaloneToolBar->action("SaveAllFiles")->setEnabled(IsWareModified);
  }

  emit modifiedStatusChanged(IsCurrentEditorModified, IsWareModified);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::goToLine()
{
  if (WareFileEditor* Editor = currentEditor())
    Editor->goToLine();
  else
    QMessageBox::warning(0, tr("No open file"), tr("No open editor"));
}


// =====================================================================
// =====================================================================


void WareSrcWidget::updateEditorsSettings()
{
  for (WareFileEditor* Editor : m_WareFilesByPath.values())
    Editor->updateSettings();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::notifyConfigureLaunched(openfluid::ware::WareType Type, const QString& ID)
{
  emit configureLaunched(Type,ID);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::notifyConfigureFinished(openfluid::ware::WareType Type, const QString& ID)
{
  emit configureFinished(Type,ID);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::notifyBuildLaunched(openfluid::ware::WareType Type, const QString& ID)
{
  emit buildLaunched(Type,ID);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::notifyBuildFinished(openfluid::ware::WareType Type, const QString& ID)
{
  emit buildFinished(Type,ID);
}


} } } // namespaces
