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
 @file MainWindow.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <QApplication>
#include <QDesktopWidget>
#include <QAction>
#include <QUrl>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QCloseEvent>

#include <openfluid/config.hpp>
#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>
#include <openfluid/ui/common/AboutDialog.hpp>
#include <openfluid/ui/common/PreferencesDialog.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/waresdev/WaresSrcExportDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardDialog.hpp>
#include <openfluid/waresdev/WaresDevPackage.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/utils/CMakeProxy.hpp>

#include "DevStudioPreferencesManager.hpp"
#include "MainWindow.hpp"
#include "ui_MainWindow.h"


MainWindow::MainWindow(openfluid::ui::common::OpenFLUIDSplashScreen* Splash) :
    QMainWindow(), ui(new Ui::MainWindow)
{
  Splash->setMessage(tr("Preparing user interface"));

  ui->setupUi(this);

  QRect ScreenRect = QApplication::desktop()->screenGeometry();
  resize(ScreenRect.width() * 0.9, ScreenRect.height() * 0.9);
  move((ScreenRect.width() - width()) / 2, (ScreenRect.height() - height()) / 2);

  QList<int> Sizes;
  Sizes << 220 << 1000;
  ui->splitter->setSizes(Sizes);

  mp_Toolbar = new openfluid::ui::waresdev::WareSrcToolbar(false, this);

  mp_Toolbar->setStyleSheet("QWidget {padding-left : 10px; padding-right : 10px;}");

  mp_Toolbar->setObjectName("SrcToolbar");
  mp_Toolbar->setStyleSheet(
      QString("QWidget "
              "{color: white; padding-left : 10px; padding-right : 10px;} "
              "#SrcToolbar "
              "{background-color: %1; border: 1px solid %1;}"
              "QToolButton[popupMode=\"1\"] "
              "{background-color: %1; border: 1px solid %1; "
              "padding-left : 10px; padding-right : 20px;}"
              "QToolButton::hover "
              "{ background-color: %2; border : 1px solid %3; border-radius: 4px; }"
              "QToolButton::menu-button "
              "{background-color: %1; border: 1px solid %1; border-radius: 4px;}"
              "QToolButton::menu-button:pressed, QToolButton::menu-button:hover "
              "{ background-color: %2; border : 1px solid %3; border-radius: 4px; }").arg(
          openfluid::ui::config::TOOLBAR_BGCOLOR, openfluid::ui::config::TOOLBARBUTTON_BGCOLOR,
          openfluid::ui::config::TOOLBARBUTTON_BORDERCOLOR));

  addToolBar(mp_Toolbar);

  Splash->setMessage(tr("Initializing sources codes management"));

  openfluid::waresdev::WareSrcManager* Manager = openfluid::waresdev::WareSrcManager::instance();

  ui->WaresTabWidget->setCurrentIndex(0);

  ui->SimExplorer->configure(Manager->getWareTypePath(openfluid::ware::WareType::SIMULATOR), true);
  ui->ObsExplorer->configure(Manager->getWareTypePath(openfluid::ware::WareType::OBSERVER), true);
  ui->ExtExplorer->configure(Manager->getWareTypePath(openfluid::ware::WareType::BUILDEREXT), true);

  mp_Collection = new openfluid::ui::waresdev::WareSrcWidgetCollection(ui->WareSrcCollection, false);


  Splash->setMessage(tr("Configuring UI"));

  createLocalActions();
  createMenus();

  connect(m_Actions["NewSimulator"], SIGNAL(triggered()), mp_Collection, SLOT(newSimulator()));
  connect(m_Actions["NewObserver"], SIGNAL(triggered()), mp_Collection, SLOT(newObserver()));
  connect(m_Actions["NewExtension"], SIGNAL(triggered()), mp_Collection, SLOT(newBuilderExtension()));
  connect(m_Actions["OpenSimulator"], SIGNAL(triggered()), mp_Collection, SLOT(openSimulator()));
  connect(m_Actions["OpenObserver"], SIGNAL(triggered()), mp_Collection, SLOT(openObserver()));
  connect(m_Actions["OpenExtension"], SIGNAL(triggered()), mp_Collection, SLOT(openBuilderExtension()));
  connect(m_Actions["SaveAsFile"], SIGNAL(triggered()), mp_Collection, SLOT(saveAsMayBeAboveWare()));
  connect(m_Actions["CloseWares"], SIGNAL(triggered()), this, SLOT(onCloseAllWaresRequested()));
  connect(m_Actions["DeleteWare"], SIGNAL(triggered()), this, SLOT(onDeleteWareRequested()));
  // TODO to enable again when developped
  //connect(m_Actions["SwitchWorkspace"], SIGNAL(triggered()), this, SLOT(showNotYetImplemented()));
  connect(m_Actions["Quit"], SIGNAL(triggered()), this, SLOT(onQuitRequested()));

  connect(m_Actions["Preferences"], SIGNAL(triggered()), this, SLOT(onPreferencesAsked()));

  connect(m_Actions["ImportWareSources"], SIGNAL(triggered()), this, SLOT(onImportWareSourcesAsked()));
  connect(m_Actions["ExportWareSources"], SIGNAL(triggered()), this, SLOT(onExportWareSourcesAsked()));
  connect(m_Actions["DevDashboard"], SIGNAL(triggered()), this, SLOT(onDevDashboardAsked()));

  connect(m_Actions["HelpAbout"], SIGNAL(triggered()), this, SLOT(onAboutAsked()));
  connect(m_Actions["HelpOnlineWeb"], SIGNAL(triggered()), this, SLOT(onOnlineWebAsked()));
  connect(m_Actions["HelpOnlineCommunity"], SIGNAL(triggered()), this, SLOT(onOnlineCommunityAsked()));

  connect(mp_Toolbar->action("NewFile"), SIGNAL(triggered()), mp_Collection, SLOT(newFile()));
  connect(mp_Toolbar->action("OpenFile"), SIGNAL(triggered()), mp_Collection, SLOT(openFile()));
  connect(mp_Toolbar->action("SaveFile"), SIGNAL(triggered()), mp_Collection, SLOT(saveCurrentEditor()));
  connect(mp_Toolbar->action("SaveAsFile"), SIGNAL(triggered()), mp_Collection, SLOT(saveAs()));
  connect(mp_Toolbar->action("SaveAllFiles"), SIGNAL(triggered()), mp_Collection, SLOT(saveAllCurrent()));
  connect(mp_Toolbar->action("CloseFile"), SIGNAL(triggered()), mp_Collection, SLOT(closeCurrentEditor()));
  connect(mp_Toolbar->action("DeleteFile"), SIGNAL(triggered()), mp_Collection, SLOT(deleteCurrentFile()));

  connect(mp_Toolbar->action("Copy"), SIGNAL(triggered()), mp_Collection, SLOT(copyText()));
  connect(mp_Toolbar->action("Cut"), SIGNAL(triggered()), mp_Collection, SLOT(cutText()));
  connect(mp_Toolbar->action("Paste"), SIGNAL(triggered()), mp_Collection, SLOT(pasteText()));
  connect(mp_Toolbar->action("FindReplace"), SIGNAL(triggered()), mp_Collection, SLOT(showFindReplaceDialog()));
  connect(mp_Toolbar->action("GoToLine"), SIGNAL(triggered()), mp_Collection, SLOT(goToLine()));

  connect(mp_Toolbar->action("Release"), SIGNAL(triggered()), mp_Collection, SLOT(setReleaseMode()));
  connect(mp_Toolbar->action("Debug"), SIGNAL(triggered()), mp_Collection, SLOT(setDebugMode()));
  connect(mp_Toolbar->action("BuildInstall"), SIGNAL(triggered()), mp_Collection, SLOT(setBuildWithInstallMode()));
  connect(mp_Toolbar->action("BuildOnly"), SIGNAL(triggered()), mp_Collection, SLOT(setBuildNoInstallMode()));
  connect(mp_Toolbar->action("Configure"), SIGNAL(triggered()), mp_Collection, SLOT(configure()));
  connect(mp_Toolbar->action("Build"), SIGNAL(triggered()), mp_Collection, SLOT(build()));
  connect(mp_Toolbar->action("OpenExplorer"), SIGNAL(triggered()), mp_Collection, SLOT(openExplorer()));
  connect(mp_Toolbar->action("OpenTerminal"), SIGNAL(triggered()), mp_Collection, SLOT(openTerminal()));

  connect(mp_Toolbar->action("APIDoc"), SIGNAL(triggered()), mp_Collection, SLOT(openAPIDoc()));

  QList<openfluid::ui::waresdev::WareSrcExplorer*> Explorers( { ui->SimExplorer, ui->ObsExplorer, ui->ExtExplorer });

  for (openfluid::ui::waresdev::WareSrcExplorer* Explorer : Explorers)
  {
    connect(Explorer, SIGNAL(doubleClicked(const QString&)), mp_Collection, SLOT(openPath(const QString&)));
    connect(Explorer, SIGNAL(clicked(const QString&)), mp_Collection, SLOT(setCurrent(const QString&)));
    connect(Explorer, SIGNAL(openExplorerAsked(const QString&)), mp_Collection, SLOT(openExplorer(const QString&)));
    connect(Explorer, SIGNAL(openTerminalAsked(const QString&)), mp_Collection, SLOT(openTerminal(const QString&)));
    connect(Explorer, SIGNAL(openPathAsked(const QString&)), mp_Collection, SLOT(openPath(const QString&)));
    connect(Explorer, SIGNAL(deleteWareAsked()), this, SLOT(onDeleteWareRequested()));
    connect(Explorer, SIGNAL(fileDeleted(const QString&)), mp_Collection, SLOT(closeEditor(const QString&)));
  }

  connect(mp_Collection, SIGNAL(currentTabChanged(const QString&)), this, SLOT(setCurrentPath(const QString&)));
  connect(mp_Collection, SIGNAL(modifiedStatusChanged(bool, bool)), this, SLOT(updateSaveButtonsStatus(bool, bool)));
  connect(mp_Collection, SIGNAL(editorSaved()), this, SLOT(updateExplorer()));

  connect(ui->CollapseAllSimActionLabel, SIGNAL(clicked()), ui->SimExplorer, SLOT(collapseAll()));
  connect(ui->CollapseAllObsActionLabel, SIGNAL(clicked()), ui->ObsExplorer, SLOT(collapseAll()));
  connect(ui->CollapseAllExtActionLabel, SIGNAL(clicked()), ui->ExtExplorer, SLOT(collapseAll()));

  Splash->setMessage(tr("Initializing workspace"));

  setWorkspaceDefaults();

  updateSaveButtonsStatus(false, false);


  QString TmpLabel = tr("Current workspace: %1")
                     .arg(QDir::toNativeSeparators(openfluid::base::PreferencesManager::instance()
                                                     ->getBuilderWorkspacePath()));
  statusBar()->addPermanentWidget(new QLabel(TmpLabel),1);
}


// =====================================================================
// =====================================================================


MainWindow::~MainWindow()
{
  openfluid::waresdev::WareSrcManager::kill();
  openfluid::base::PreferencesManager::kill();
  openfluid::base::RunContextManager::kill();
  delete ui;
}


// =====================================================================
// =====================================================================


void MainWindow::createLocalActions()
{
  m_Actions["NewSimulator"] = new QAction(tr("Simulator..."), this);
  m_Actions["NewObserver"] = new QAction(tr("Observer..."), this);
  m_Actions["NewExtension"] = new QAction(tr("Builder extension..."), this);

  m_Actions["OpenSimulator"] = new QAction(tr("Simulator..."), this);
  m_Actions["OpenObserver"] = new QAction(tr("Observer..."), this);
  m_Actions["OpenExtension"] = new QAction(tr("Builder extension..."), this);

  m_Actions["SaveAsFile"] = new QAction(openfluid::ui::common::getIcon("file-save-as","/ui/common"),
                                        tr("Save as..."), this);
  m_Actions["SaveAsFile"]->setToolTip(tr("Save the current file as..."));

  m_Actions["CloseWares"] = new QAction(tr("Close all wares"), this);

  m_Actions["DeleteWare"] = new QAction(tr("Delete ware"), this);

  // TODO to enable again when developped
  //m_Actions["SwitchWorkspace"] = new QAction(tr("Switch workspace"), this);

  m_Actions["Quit"] = new QAction(tr("Quit"), this);
  m_Actions["Quit"]->setShortcuts(QKeySequence::Quit);

  /* TODO check it, because :
   "The menu role can only be changed before the actions are put into the menu bar in Mac OS X
   (usually just before the first application window is shown)."
   (http://qt-project.org/doc/qt-4.8/qaction.html#menuRole-prop)*/
  m_Actions["Quit"]->setMenuRole(QAction::QuitRole);

  m_Actions["Preferences"] = new QAction(tr("Preferences..."), this);
  m_Actions["Preferences"]->setMenuRole(QAction::PreferencesRole);


  // Tools menu
  m_Actions["ImportWareSources"] = new QAction(tr("Import wares sources..."), this);
  m_Actions["ExportWareSources"] = new QAction(tr("Export wares sources..."), this);
  m_Actions["GitDashboard"] = new QAction(tr("Dashboard..."), this);
  m_Actions["DevDashboard"] = new QAction(tr("Development dashboard..."), this);
  m_Actions["DevDashboard"]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_D));

  //Help menu
  m_Actions["HelpOnlineWeb"] = new QAction(tr("Web site"), this);
  m_Actions["HelpOnlineCommunity"] = new QAction(tr("Community site"), this);
  m_Actions["HelpEmail"] = new QAction(tr("Email"), this);

  m_Actions["HelpAbout"] = new QAction(tr("About"), this);
  m_Actions["HelpAbout"]->setMenuRole(QAction::AboutRole);
}


// =====================================================================
// =====================================================================


void MainWindow::createMenus()
{
  QMenu* Menu;
  QMenu* SubMenu;

  Menu = menuBar()->addMenu(tr("File"));
  SubMenu = Menu->addMenu(tr("New ware"));
  SubMenu->addAction(m_Actions["NewSimulator"]);
  SubMenu->addAction(m_Actions["NewObserver"]);
  SubMenu->addAction(m_Actions["NewExtension"]);
  SubMenu = Menu->addMenu(tr("Open ware"));
  SubMenu->addAction(m_Actions["OpenSimulator"]);
  SubMenu->addAction(m_Actions["OpenObserver"]);
  SubMenu->addAction(m_Actions["OpenExtension"]);
  Menu->addAction(m_Actions["CloseWares"]);
  Menu->addAction(m_Actions["DeleteWare"]);
  Menu->addSeparator();
  Menu->addAction(mp_Toolbar->action("NewFile"));
  Menu->addAction(mp_Toolbar->action("OpenFile"));
  Menu->addAction(mp_Toolbar->action("SaveFile"));
  Menu->addAction(m_Actions["SaveAsFile"]);
  Menu->addAction(mp_Toolbar->action("SaveAllFiles"));
  Menu->addAction(mp_Toolbar->action("CloseFile"));
  Menu->addAction(mp_Toolbar->action("DeleteFile"));
  Menu->addSeparator();
  // TODO to enable again when developped
  //Menu->addAction(m_Actions.value("SwitchWorkspace"));
  Menu->addAction(m_Actions.value("Quit"));

  Menu = menuBar()->addMenu(tr("Edit"));
  Menu->addAction(mp_Toolbar->action("Copy"));
  Menu->addAction(mp_Toolbar->action("Cut"));
  Menu->addAction(mp_Toolbar->action("Paste"));
  Menu->addAction(mp_Toolbar->action("FindReplace"));
  Menu->addAction(mp_Toolbar->action("GoToLine"));
  Menu->addSeparator();
  Menu->addAction(m_Actions["Preferences"]);

  Menu = menuBar()->addMenu(tr("Build"));
  Menu->addAction(mp_Toolbar->action("Configure"));
  SubMenu = Menu->addMenu(tr("Active configuration"));
  SubMenu->addAction(mp_Toolbar->action("Release"));
  SubMenu->addAction(mp_Toolbar->action("Debug"));
  Menu->addAction(mp_Toolbar->action("Build"));
  SubMenu = Menu->addMenu(tr("Active build action"));
  SubMenu->addAction(mp_Toolbar->action("BuildInstall"));
  SubMenu->addAction(mp_Toolbar->action("BuildOnly"));

  Menu = menuBar()->addMenu(tr("Tools"));
  Menu->addAction(mp_Toolbar->action("OpenTerminal"));
  Menu->addAction(mp_Toolbar->action("OpenExplorer"));
  Menu->addSeparator();
  Menu->addAction(m_Actions["ImportWareSources"]);
  Menu->addAction(m_Actions["ExportWareSources"]);
  Menu->addSeparator();
  Menu->addAction(m_Actions["DevDashboard"]);

  Menu = menuBar()->addMenu(tr("&Help"));
  SubMenu = Menu->addMenu(tr("OpenFLUID online"));
  SubMenu->addAction(m_Actions["HelpOnlineWeb"]);
  SubMenu->addAction(m_Actions["HelpOnlineCommunity"]);
  SubMenu->addAction(mp_Toolbar->action("APIDoc"));
  Menu->addSeparator();
  Menu->addAction(m_Actions["HelpAbout"]);

}


// =====================================================================
// =====================================================================


void MainWindow::setWorkspaceDefaults()
{
  DevStudioPreferencesManager* Mgr = DevStudioPreferencesManager::instance();

  QStringList Mode = Mgr->getConfigBuildMode().split("|");
  mp_Toolbar->action(Mode.contains("DEBUG", Qt::CaseInsensitive) ? "Debug" : "Release")->trigger();
  mp_Toolbar->action(Mode.contains("BUILDONLY", Qt::CaseInsensitive) ? "BuildOnly" : "BuildInstall")->trigger();

  QStringList LastOpenWares = Mgr->getLastOpenWares();

  for (QString WarePath : LastOpenWares)
    mp_Collection->openPath(WarePath);

  mp_Collection->setCurrent(Mgr->getLastActiveWare());
}


// =====================================================================
// =====================================================================


void MainWindow::showNotYetImplemented()
{
  QMessageBox::information(0, "", "Not yet implemented");
}


// =====================================================================
// =====================================================================


void MainWindow::onQuitRequested()
{
  if (QMessageBox::question(this,
                            tr("Quit"),tr("Are you sure you want to quit OpenFLUID-DevStudio?"),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    DevStudioPreferencesManager* Mgr = DevStudioPreferencesManager::instance();

    QStringList Mode;
    Mode << (mp_Collection->isDebugMode() ? "DEBUG" : "RELEASE");
    Mode << (mp_Collection->isBuildNoInstallMode() ? "BUILDONLY" : "BUILDINSTALL");
    Mgr->setConfigBuildMode(Mode.join("|"));

    Mgr->setLastOpenWares(mp_Collection->getOpenWarePaths());

    Mgr->setLastActiveWare(mp_Collection->getCurrentWarePath());

    if (mp_Collection->closeAllWidgets())
      qApp->quit();
  }
}


// =====================================================================
// =====================================================================


void MainWindow::onPreferencesAsked()
{
  openfluid::ui::common::PreferencesDialog PrefsDlg(QApplication::activeWindow(),
                                                    openfluid::ui::common::PreferencesDialog::MODE_DEVSTUDIO);

  connect(&PrefsDlg, SIGNAL(applyTextEditorSettingsAsked()), mp_Collection, SLOT(updateEditorsSettings()));

  PrefsDlg.exec();

  if (PrefsDlg.isTextEditorSettingsChanged())
    mp_Collection->updateEditorsSettings();

  if (PrefsDlg.isRestartRequired() &&
      openfluid::ui::common::OpenFLUIDDialog::confirmRestartAfterPreferences(this))
  {
    QCoreApplication::exit(openfluid::ui::config::EXIT_CODE_FOR_RESTART);
  }
}


// =====================================================================
// =====================================================================


void MainWindow::onImportWareSourcesAsked()
{
  if (openfluid::utils::CMakeProxy::isAvailable() || openfluid::utils::GitProxy::isAvailable())
  {
    openfluid::ui::waresdev::WaresSrcImportDialog Dialog(this);
    Dialog.exec();
  }
  else
    QMessageBox::warning(this, tr("Import not available"), tr("Neither CMake program nor Git program can be found."));
}


// =====================================================================
// =====================================================================


void MainWindow::onExportWareSourcesAsked()
{
  if (openfluid::utils::CMakeProxy::isAvailable())
  {
    openfluid::ui::waresdev::WaresSrcExportDialog Dialog(this);
    Dialog.exec();
  }
  else
    QMessageBox::warning(this, tr("Export not available"), tr("CMake program can not be found."));

}


// =====================================================================
// =====================================================================


void MainWindow::onDevDashboardAsked()
{
  openfluid::ui::waresdev::WorkspaceDevDashboardDialog Dialog(this);
  Dialog.setWindowTitle(tr("Development dashboard"));

  Dialog.exec();
}


// =====================================================================
// =====================================================================


void MainWindow::onOnlineWebAsked()
{
  QDesktopServices::openUrl(QUrl(QString::fromStdString(openfluid::config::WEBSITE_URL), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void MainWindow::onOnlineCommunityAsked()
{
  QDesktopServices::openUrl(QUrl(QString::fromStdString(openfluid::config::COMMUNITY_URL), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void MainWindow::onAboutAsked()
{
  openfluid::ui::common::AboutDialog AboutDlg(this, m_Actions["HelpOnlineWeb"], m_Actions["HelpEmail"]);

  AboutDlg.exec();
}


// =====================================================================
// =====================================================================


void MainWindow::closeEvent(QCloseEvent* Event)
{
  Event->ignore();
  onQuitRequested();
}


// =====================================================================
// =====================================================================


void MainWindow::setCurrentPath(const QString& Path)
{
  if (ui->SimExplorer->setCurrentPath(Path))
    ui->WaresTabWidget->setCurrentWidget(ui->SimPage);
  else if (ui->ObsExplorer->setCurrentPath(Path))
    ui->WaresTabWidget->setCurrentWidget(ui->ObsPage);
  else if (ui->ExtExplorer->setCurrentPath(Path))
    ui->WaresTabWidget->setCurrentWidget(ui->ExtPage);
}


// =====================================================================
// =====================================================================


void MainWindow::updateSaveButtonsStatus(bool FileModified, bool WareModified)
{
  mp_Toolbar->action("SaveFile")->setEnabled(FileModified);
  mp_Toolbar->action("SaveAsFile")->setEnabled(FileModified);
  mp_Toolbar->action("SaveAllFiles")->setEnabled(WareModified);
  m_Actions["SaveAsFile"]->setEnabled(FileModified);
}


// =====================================================================
// =====================================================================


void MainWindow::onDeleteWareRequested()
{
  QWidget* CurrentWidget = ui->WaresTabWidget->currentWidget();
  QString SelectedPath = "";

  if (CurrentWidget == ui->SimPage)
    SelectedPath = ui->SimExplorer->getCurrentPath();
  else if (CurrentWidget == ui->ObsPage)
    SelectedPath = ui->ObsExplorer->getCurrentPath();
  else if (CurrentWidget == ui->ExtPage)
    SelectedPath = ui->ExtExplorer->getCurrentPath();

  if (SelectedPath != "")
  {
    QString WarePath = openfluid::waresdev::WareSrcManager::instance()->getPathInfo(SelectedPath).m_AbsolutePathOfWare;
    if (WarePath != "")
      mp_Collection->deleteWare(WarePath);
  }
}



// =====================================================================
// =====================================================================


void MainWindow::onCloseAllWaresRequested()
{
  mp_Collection->closeAllWidgets();
}


// =====================================================================
// =====================================================================


void MainWindow::updateExplorer()
{
  QString CurrentWarePath = mp_Collection->getCurrentWarePath();
  QWidget* CurrentWidget = ui->WaresTabWidget->currentWidget();

  if (CurrentWidget == ui->SimPage)
    ui->SimExplorer->updateExplorerModel(CurrentWarePath);
  else if (CurrentWidget == ui->ObsPage)
    ui->ObsExplorer->updateExplorerModel(CurrentWarePath);
  else if (CurrentWidget == ui->ExtPage)
    ui->ExtExplorer->updateExplorerModel(CurrentWarePath);
}

