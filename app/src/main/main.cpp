/**
  \file main.cpp
  \brief implements main

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <iostream>

//#include <wx/log.h>


#include "AppTools.h"
#include "main.h"
#include "setup.h"

using namespace mhydasdk::base;
using namespace mhydasdk::core;




// =====================================================================
// =====================================================================


IMPLEMENT_APP_CONSOLE(MHYDASApp);

// =====================================================================
// =====================================================================


bool MHYDASApp::buildModel()
{
  bool ExecStatus;

  std::cout << "* Building model... ";
  std::cout.flush();

  mp_Engine->buildModel();

  printlnExecStatus();

  if (!mp_ExecMsgs->isErrorFlag())
  {
    if (mp_Engine->getConfig().SimulationID != wxT("")) m_ExSI.SimID = mp_Engine->getConfig().SimulationID;
    else m_ExSI.SimID = GenerateSimulationID();
  }  

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::loadData()
{
  bool ExecStatus;

  std::cout << "* Loading data... ";
  std::cout.flush();

  ExecStatus = mp_Engine->loadData();
  
  printlnExecStatus();

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::checkConsistency()
{

  bool ExecStatus;

  std::cout << "* Preparing data and checking consistency... ";
  std::cout.flush();
 
  
  ExecStatus = mp_Engine->prepareDataAndCheckConsistency();
  
  
  printlnExecStatus();

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::runSimulation()
{

  bool ExecStatus;

  printDataInfos();

  std::cout << "**** Running simulation ****" << endl;
  std::cout.flush();

  m_EffectiveStartTime = wxDateTime::Now();

  ExecStatus = mp_Engine->run();

  m_EffectiveEndTime = wxDateTime::Now();

  if (!mp_ExecMsgs->isErrorFlag()) std::cout << "**** Simulation completed ****" << endl << endl;
  else  std::cout << "**** Simulation aborted ****" << endl;
  std::cout.flush();

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::saveResults()
{

  bool ExecStatus;

  std::cout << "* Saving results... ";
  std::cout.flush();

  ExecStatus = mp_Engine->saveResults(m_ExSI);

  printlnExecStatus();

  return ExecStatus;
}

// =====================================================================
// =====================================================================

bool MHYDASApp::saveSimulationReports()
{

  bool ExecStatus;

  std::cout << "* Saving simulation report... ";
  std::cout.flush();

  ExecStatus = mp_Engine->saveReports(m_ExSI);

  std::cout << "[Done]" << std::endl;

  std::cout.flush();

  return ExecStatus;  
  
  return true;
}


// =====================================================================
// =====================================================================


void MHYDASApp::printlnExecStatus()
{
  if (!mp_ExecMsgs->isErrorFlag())
  {
    if (mp_ExecMsgs->isWarningFlag()) std::cout << "[Warning]" << std::endl;
    else std::cout << "[OK]" << std::endl;
  }
  else std::cout << "[Error]" << std::endl;
    
  std::cout.flush();
}

// =====================================================================
// =====================================================================


void MHYDASApp::printlnExecMessagesStats()
{
  if (mp_ExecMsgs->isErrorFlag()) std::cout << "1 error, ";
  else  std::cout << "no error, ";
  std::cout << mp_ExecMsgs->getWarningMsgs().Count() << " warning(s)" << std::endl; 
}

// =====================================================================
// =====================================================================


void MHYDASApp::printMHYDASInfos()
{
  
  int Width = 60;
  wxString VersionInfo = wxT("v ");
  wxString Whites = wxT("");

  VersionInfo = VersionInfo + MAJOR_VERSION + wxT(".") + MINOR_VERSION + wxT("-") + SVN_REVISION + RELEASE_STATUS;

  
  if (BUILD_VERSION != wxT(""))
  {
    
    VersionInfo = VersionInfo + wxT(" (") + BUILD_VERSION + wxT(" build");
     
    #ifdef SVN_REV
    VersionInfo = VersionInfo + wxT(", rev ") + SVN_REVISION;
    #endif
    VersionInfo = VersionInfo + wxT(")");
  }
  
  // on centre le bizness avec des blancs
  for (int i=0;i<((Width-VersionInfo.Length())/2);i++) Whites = Whites + wxT(" ");
  
  VersionInfo = Whites + VersionInfo;
  
  
  std::cout << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << "                       XXXXX-engine                        " << std::endl;
  std::cout << "                Landscape modelling system                 " << std::endl;
  std::cout << std::endl;
  std::cout << VersionInfo.mb_str(wxConvUTF8) << std::endl;
  std::cout << std::endl;  
  std::cout << "                 LISAH, Montpellier, France                " << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << std::endl;
  std::cout.flush();
}



// =====================================================================
// =====================================================================

void MHYDASApp::printDataInfos()
{
  // int TimeStepsNbr = (mp_CoreData->getRainEvent()->getEventEndingTime().getRawTime() - mp_CoreData->getRainEvent()->getEventStartingTime().getRawTime()) / mp_Engine->getConfig().DeltaT; 
  
  std::cout << std::endl;
  std::cout << "Simulation ID: " << _C(m_ExSI.SimID) << std::endl;
  std::cout << std::endl;  
  std::cout << "Spatial objects: " << std::endl
            << "   - " << mp_CoreData->getSpatialData()->getSUsCollection()->size() << " Surface Units" << std::endl
            << "   - " << mp_CoreData->getSpatialData()->getRSsCollection()->size() << " Reach Segments" << std::endl
            << "   - " << mp_CoreData->getSpatialData()->getGUsCollection()->size() << " Groundwater Units" << std::endl;
  std::cout << "Rain source(s): " << mp_CoreData->getRainSources()->getRainSourceCollection().size() << std::endl;
  std::cout << "Simulation from " << _C(mp_Engine->getSimulationInfo()->getStartTime().asString())
            << " to " << _C(mp_Engine->getSimulationInfo()->getEndTime().asString()) << std::endl
            << "         -> " <<  (mp_Engine->getSimulationInfo()->getStepsCount()) << " time steps of " << mp_Engine->getSimulationInfo()->getTimeStep() << " seconds" << std::endl;
  std::cout << std::endl;
  std::cout.flush();

}


// =====================================================================
// =====================================================================

void MHYDASApp::printPluginsList()
{  std::cout << "Simulation ID: " << _C(m_ExSI.SimID) << std::endl;

  ArrayOfPluginsContainers PlugContainers = mp_PlugMan->getAvailableFunctions();

  std::cout << "Available pluggable simulation functions:" << std::endl;


  if (PlugContainers.GetCount() > 0)
  {
    for (int i=0;i<PlugContainers.GetCount();i++) std::cout << "  - " << ReplaceEmptyString(PlugContainers[i]->Signature->Name,wxT("(unknown simulation function)")).mb_str(wxConvUTF8) << std::endl;
  }
  else
  {
    std::cout << "  (none)" << std::endl;
  }
  std::cout << std::endl;
  std::cout.flush();

}


// =====================================================================
// =====================================================================



void MHYDASApp::printPluginsHandledDataItemReport(mhydasdk::base::SignatureHandledItem HandledItem, wxString Suffix, wxString Type, bool IsXMLFormat)
{
  wxString TypeStr = wxT("");

  std::cout << Suffix.mb_str(wxConvUTF8);

  if (IsXMLFormat)
  {
    std::cout << "<varpropparam";
    std::cout << " type=\"" << Type.mb_str(wxConvUTF8) << "\"";
    std::cout << " distribution=\"" << HandledItem.Distribution.mb_str(wxConvUTF8) << "\"";        
    std::cout << " ID=\"" << HandledItem.Name.mb_str(wxConvUTF8) << "\"";                
    std::cout << " description=\"" << HandledItem.Description.mb_str(wxConvUTF8) << "\"";
    std::cout << " unit=\"" << HandledItem.Unit.mb_str(wxConvUTF8) << "\"";        
    std::cout << "/>" << std::endl;
      
  }
  else
  {
    wxString UnitStr = wxT("");
    wxString DistribStr = wxT("");
    
        
    if (HandledItem.Unit != wxT("")) UnitStr = wxT(" (")+HandledItem.Unit+wxT(")");
    if (HandledItem.Distribution != wxT("")) DistribStr = wxT(", distributed on ")+HandledItem.Distribution;
    
    
    if (Type == wxT("pvar")) TypeStr = wxT("produced variable");
    if (Type == wxT("uvar")) TypeStr = wxT("updated variable");    
    
    if (Type == wxT("rvar")) TypeStr = wxT("required variable");
    if (Type == wxT("rprevvar")) TypeStr = wxT("required variable produced at previous step");
    if (Type == wxT("svar")) TypeStr = wxT("used variable (only if available)");    
    if (Type == wxT("sprevvar")) TypeStr = wxT("used variable produced at previous step (only if available)");
    
    if (Type == wxT("fpar")) TypeStr = wxT("function parameter");
    
    if (Type == wxT("rprop")) TypeStr = wxT("required distributed property");
    if (Type == wxT("sprop")) TypeStr = wxT("used distributed property (only if available)");
    if (Type == wxT("rinicond")) TypeStr = wxT("required distributed initial condition");
    if (Type == wxT("sinicond")) TypeStr = wxT("used distributed initial condition (only if available)");
    
    std::cout << HandledItem.Name.mb_str(wxConvUTF8) << UnitStr.mb_str(wxConvUTF8) << " : " << TypeStr.mb_str(wxConvUTF8) << DistribStr.mb_str(wxConvUTF8) << ".";
    if (HandledItem.Description.Length()!=0) std::cout << " " << HandledItem.Description.mb_str(wxConvUTF8);     
    std::cout << std::endl;
    
  }
  
  
}



// =====================================================================
// =====================================================================


void MHYDASApp::printPluginsHandledDataReport(mhydasdk::base::SignatureHandledData HandledData, wxString Suffix, bool IsXMLFormat)
{
  
  int i;

  for (i=0;i<HandledData.FunctionParams.size();i++) printPluginsHandledDataItemReport(HandledData.FunctionParams[i],Suffix,wxT("fpar"),IsXMLFormat);
  for (i=0;i<HandledData.ProducedVars.size();i++) printPluginsHandledDataItemReport(HandledData.ProducedVars[i],Suffix,wxT("pvar"),IsXMLFormat);  
  for (i=0;i<HandledData.RequiredVars.size();i++) printPluginsHandledDataItemReport(HandledData.RequiredVars[i],Suffix,wxT("rvar"),IsXMLFormat);  
  for (i=0;i<HandledData.UpdatedVars.size();i++) printPluginsHandledDataItemReport(HandledData.UpdatedVars[i],Suffix,wxT("uvar"),IsXMLFormat);
  for (i=0;i<HandledData.UsedVars.size();i++) printPluginsHandledDataItemReport(HandledData.UsedVars[i],Suffix,wxT("svar"),IsXMLFormat);
  for (i=0;i<HandledData.RequiredPrevVars.size();i++) printPluginsHandledDataItemReport(HandledData.RequiredPrevVars[i],Suffix,wxT("rprevvar"),IsXMLFormat);
  for (i=0;i<HandledData.UsedPrevVars.size();i++) printPluginsHandledDataItemReport(HandledData.UsedPrevVars[i],Suffix,wxT("sprevvar"),IsXMLFormat);
  for (i=0;i<HandledData.RequiredProps.size();i++) printPluginsHandledDataItemReport(HandledData.RequiredProps[i],Suffix,wxT("rprop"),IsXMLFormat);
  for (i=0;i<HandledData.UsedProps.size();i++) printPluginsHandledDataItemReport(HandledData.UsedProps[i],Suffix,wxT("sprop"),IsXMLFormat);
  for (i=0;i<HandledData.RequiredIniconds.size();i++) printPluginsHandledDataItemReport(HandledData.RequiredIniconds[i],Suffix,wxT("rinicond"),IsXMLFormat);
  for (i=0;i<HandledData.UsedIniconds.size();i++) printPluginsHandledDataItemReport(HandledData.UsedIniconds[i],Suffix,wxT("sinicond"),IsXMLFormat);

  if (IsXMLFormat)
  {
    std::cout << Suffix.mb_str(wxConvUTF8) << "<usedevents SU=\"" << HandledData.UsedEventsOnSU << "\" RS=\"" << HandledData.UsedEventsOnRS << "\" GU=\"" << HandledData.UsedEventsOnGU << "\"/>" << std::endl;

    for (i=0;i<HandledData.RequiredExtraFiles.GetCount();i++) std::cout << Suffix.mb_str(wxConvUTF8) << "<extrafile type=\"required\" name=\"" << HandledData.RequiredExtraFiles[i].mb_str(wxConvUTF8) << "\" />" << std::endl;   
    for (i=0;i<HandledData.UsedExtraFiles.GetCount();i++) std::cout << Suffix.mb_str(wxConvUTF8) << "<extrafile type=\"used\" name=\"" << HandledData.UsedExtraFiles[i].mb_str(wxConvUTF8) << "\" />" << std::endl;
    
  }
  else
  {
    
    wxString EventsSUStr = wxT("no");
    wxString EventsRSStr = wxT("no");
    wxString EventsGUStr = wxT("no");
    
    if (HandledData.UsedEventsOnSU) EventsSUStr = wxT("yes");
    if (HandledData.UsedEventsOnRS) EventsRSStr = wxT("yes");
    if (HandledData.UsedEventsOnGU) EventsGUStr = wxT("yes");
        
    std::cout << Suffix.mb_str(wxConvUTF8) << "Events used on SUs : " << EventsSUStr.mb_str(wxConvUTF8) << std::endl;
    std::cout << Suffix.mb_str(wxConvUTF8) << "Events used on RSs : " << EventsRSStr.mb_str(wxConvUTF8) << std::endl;
    std::cout << Suffix.mb_str(wxConvUTF8) << "Events used on GUs : " << EventsGUStr.mb_str(wxConvUTF8) << std::endl;
        
    
    for (i=0;i<HandledData.RequiredExtraFiles.GetCount();i++) std::cout << Suffix.mb_str(wxConvUTF8) << "Required extra file : " << HandledData.RequiredExtraFiles[i].mb_str(wxConvUTF8) << std::endl;    
    for (i=0;i<HandledData.UsedExtraFiles.GetCount();i++) std::cout << Suffix.mb_str(wxConvUTF8) << "Used extra file : " << HandledData.UsedExtraFiles[i].mb_str(wxConvUTF8) << std::endl;
    
  }
  
}


// =====================================================================
// =====================================================================

void MHYDASApp::printPluginsReport(bool IsXMLFormat)
{

 
  ArrayOfPluginsContainers PlugContainers = mp_PlugMan->getAvailableFunctions(); 
  wxString StatusStr;
  
  
  // insertion du début du fichier XML
  if (IsXMLFormat)
  {
    std::cout << "<?xml version=\"1.0\" standalone=\"yes\"?>" << std::endl;
    std::cout << "<mhydas>" << std::endl;   
    std::cout << "  <funcsreport>" << std::endl;   
  }
  

  
  if (PlugContainers.GetCount() > 0)
  {
    for (int i=0;i<PlugContainers.GetCount();i++)
    {
      
      // Status string
      StatusStr = wxT("experimental");
      if (PlugContainers[i]->Signature->Status == mhydasdk::base::BETA) StatusStr = wxT("beta");
      if (PlugContainers[i]->Signature->Status == mhydasdk::base::STABLE) StatusStr = wxT("stable");      
      
      if (IsXMLFormat)
      {
        std::cout << "    <funcdef fileID=\"" << PlugContainers[i]->Signature->ID.mb_str(wxConvUTF8) 
                  << "\" name=\"" << PlugContainers[i]->Signature->Name.mb_str(wxConvUTF8) << "\">" << std::endl;
        std::cout << "      <file>" << PlugContainers[i]->Filename.mb_str(wxConvUTF8) << "</file>" << std::endl;
        std::cout << "      <domain>" << PlugContainers[i]->Signature->Domain.mb_str(wxConvUTF8) << "</domain>" << std::endl;
        std::cout << "      <process>" << PlugContainers[i]->Signature->Process.mb_str(wxConvUTF8) << "</process>" << std::endl;
        std::cout << "      <method>" << PlugContainers[i]->Signature->Method.mb_str(wxConvUTF8) << "</method>" << std::endl;
        std::cout << "      <description>" << PlugContainers[i]->Signature->Description.mb_str(wxConvUTF8) << "</description>" << std::endl;
        std::cout << "      <version number=\"" << PlugContainers[i]->Signature->Version.mb_str(wxConvUTF8) << "\" sdk=\""<< PlugContainers[i]->Signature->SDKVersion.mb_str(wxConvUTF8) << "\" devstatus=\"" << StatusStr.mb_str(wxConvUTF8) << "\"/>" << std::endl;
        std::cout << "      <author name=\"" << PlugContainers[i]->Signature->Author.mb_str(wxConvUTF8) 
                  << "\" email=\"" << PlugContainers[i]->Signature->AuthorEmail.mb_str(wxConvUTF8) << "\"/>" << std::endl;

        std::cout << "      <handleddata>" << std::endl;                  
        printPluginsHandledDataReport(PlugContainers[i]->Signature->HandledData,wxT("        "),IsXMLFormat);
        std::cout << "      </handleddata>" << std::endl;                  
                 
      }
      else
      {       
        std::cout << "* " << PlugContainers[i]->Signature->ID.mb_str(wxConvUTF8) << std::endl;           
        std::cout << "   - Name: " << ReplaceEmptyString(PlugContainers[i]->Signature->Name,wxT("(unknown)")).mb_str(wxConvUTF8) << std::endl;        
        std::cout << "   - File: " << PlugContainers[i]->Filename.mb_str(wxConvUTF8) << std::endl;        
        std::cout << "   - Domain: " << ReplaceEmptyString(PlugContainers[i]->Signature->Domain,wxT("(unknown)")).mb_str(wxConvUTF8) << std::endl;
        std::cout << "   - Process: " << ReplaceEmptyString(PlugContainers[i]->Signature->Process,wxT("(unknown)")).mb_str(wxConvUTF8) << std::endl;
        std::cout << "   - Method: " << ReplaceEmptyString(PlugContainers[i]->Signature->Method,wxT("(unknown)")).mb_str(wxConvUTF8) << std::endl;                
        std::cout << "   - Description: " << ReplaceEmptyString(PlugContainers[i]->Signature->Description,wxT("(none)")).mb_str(wxConvUTF8) << std::endl;                
        std::cout << "   - Version: " << ReplaceEmptyString(PlugContainers[i]->Signature->Method,wxT("(unknown)")).mb_str(wxConvUTF8) << std::endl;                        
        std::cout << "   - SDK version used at build time: " << PlugContainers[i]->Signature->SDKVersion.mb_str(wxConvUTF8) <<  std::endl;
        std::cout << "   - Development status: " << StatusStr.mb_str(wxConvUTF8) <<  std::endl;        
        std::cout << "   - Author(s): " << ReplaceEmptyString(PlugContainers[i]->Signature->Author,wxT("(unknown)")).mb_str(wxConvUTF8) << std::endl;                                
        std::cout << "   - Author(s) email(s) : " << ReplaceEmptyString(PlugContainers[i]->Signature->AuthorEmail,wxT("(unknown)")).mb_str(wxConvUTF8) << std::endl;
        std::cout << "   - Handled data" << std::endl;                  
        printPluginsHandledDataReport(PlugContainers[i]->Signature->HandledData,wxT("     . "),IsXMLFormat);
        
      }

      if (IsXMLFormat)
      {
        std::cout << "    </funcdef> " << std::endl;
      }  
      else
      {
        if (i != PlugContainers.GetCount()-1)
          std::cout << "================================================================================" << std::endl;
      }        
    }
     
  }
  else
  {

  }

  // insertion de la fin du fichier XML
  if (IsXMLFormat)
  {
    std::cout << "  </funcsreport>" << std::endl;   
    std::cout << "</mhydas>" << std::endl;   
  }

  std::cout.flush();

  
}

// =====================================================================
// =====================================================================

int MHYDASApp::stopAppReturn()
{  
  std::cout << std::endl;
  printlnExecMessagesStats();

  if (mp_RunEnv->isWriteSimReport())
  {  
    saveSimulationReports();
  }
  
  std::cout << "ERROR: " << FormatExecutionMessage(mp_ExecMsgs->getErrorMsg()).mb_str(wxConvUTF8) << std::endl;

  /*
  for (int i=0; i<mp_ExecMsgs->getWarningMsgs().Count();i++)
  {
    std::cout << "WARNING: " << FormatExecutionMessage(mp_ExecMsgs->getWarningMsgs().Item(i)).mb_str(wxConvUTF8) << std::endl;
  }  
  */
  
  std::cout << std::endl;
  std::cout.flush();

  return 127;
}

// =====================================================================
// =====================================================================



bool MHYDASApp::OnInit()
{
  wxString TmpStr;

  // =========== initialization ===========

  m_OKToRun = true;

  SetAppName(MHYDAS_APPNAME);

  mp_RunEnv = new RuntimeEnvironment(wxPathOnly(GetExecutablePath()));
  
  mp_ExecMsgs = new ExecutionMessages();

  mp_PlugMan = new PluginManager(mp_ExecMsgs,mp_RunEnv);

  wxLog::SetTimestamp(NULL);



  // =========== command line parsing ===========

  wxCmdLineParser Parser(CmdLineDesc,argc,argv);

  if (Parser.Parse() != 0)
  {
     std::cout << std::endl;
     return false;
  }


  if (Parser.Found(wxT("f")) || Parser.Found(wxT("k")) || Parser.Found(wxT("r")) || Parser.Found(wxT("v")) || Parser.Found(wxT("x")))
  {
    if (Parser.Found(wxT("f")))
    {
      printMHYDASInfos();
      printPluginsList();
    }  

    if (Parser.Found(wxT("r"))) printPluginsReport(false);
    if (Parser.Found(wxT("x"))) printPluginsReport(true);

    if (Parser.Found(wxT("v"))) std::cout << MAJOR_VERSION.mb_str(wxConvUTF8) << "." << MINOR_VERSION.mb_str(wxConvUTF8) << "-" << SVN_REVISION.mb_str(wxConvUTF8) << std::endl;   
    if (Parser.Found(wxT("k"))) std::cout << MHYDASDK_MAJORVER << "." << MHYDASDK_MINORVER << "-" << MHYDASDK_REVISION << std::endl;

    m_OKToRun = false;
  }
  else
  {
    if (Parser.Found(wxT("i"),&TmpStr)) mp_RunEnv->setInputDir(TmpStr);
    if (Parser.Found(wxT("o"),&TmpStr)) mp_RunEnv->setOutputDir(TmpStr);
    if (Parser.Found(wxT("m"),&TmpStr)) mp_RunEnv->setTraceDir(TmpStr);
	    
    if (Parser.Found(wxT("a"))) mp_RunEnv->setDateTimeOutputDir();
    if (Parser.Found(wxT("c"))) mp_RunEnv->setClearOutputDir(true);
    if (Parser.Found(wxT("q"))) mp_RunEnv->setQuietRun(true);
    if (Parser.Found(wxT("s"))) mp_RunEnv->setWriteSimReport(false);    
    if (Parser.Found(wxT("t"))) mp_RunEnv->setTraceMode(true);           
    if (Parser.Found(wxT("z"))) mp_RunEnv->setWriteResults(false);
    if (Parser.Found(wxT("no-varname-check"))) mp_RunEnv->setCheckVarNames(false);
	  
	  printMHYDASInfos();
	  printEnvInfos();
       
  }

  return true;
}

// =====================================================================
// =====================================================================

void MHYDASApp::printEnvInfos()
{
  std::cout << "Input dir: " << _C(mp_RunEnv->getInputDir()) << std::endl;
  if (mp_RunEnv->isWriteResults() || mp_RunEnv->isWriteSimReport()) std::cout << "Output dir: " << _C(mp_RunEnv->getOutputDir()) << std::endl;  
  if ((mp_RunEnv->isWriteResults() || mp_RunEnv->isWriteSimReport()) && (mp_RunEnv->isClearOutputDir())) std::cout << "Output dir cleared before data saving" << std::endl; 
  if (mp_RunEnv->isTraceMode()) std::cout << "Trace mode enabled" << std::endl;
  if (mp_RunEnv->isQuietRun()) std::cout << "Quiet mode enabled" << std::endl;
  if (!mp_RunEnv->isCheckVarNames()) std::cout << "Variable names checking disabled" << std::endl;  
  std::cout << std::endl; 
}


// =====================================================================
// =====================================================================

int MHYDASApp::OnRun()
{

  if (m_OKToRun)
  {

    m_TotalStartTime = wxDateTime::Now();
    m_ExSI.StartTime = m_TotalStartTime;

    mp_CoreData = new CoreRepository();

    mp_Engine = new Engine(mp_CoreData,mp_ExecMsgs,mp_RunEnv,mp_PlugMan);


    mp_CoreData->getRainSources()->enableFirstSerieConstraint(true);

    // model load and check    
    buildModel();
    if (mp_ExecMsgs->isErrorFlag()) return stopAppReturn();
    mp_ExecMsgs->resetWarningFlag();
        
    // input data load and check
    loadData();
    if (mp_ExecMsgs->isErrorFlag()) return stopAppReturn();
    mp_ExecMsgs->resetWarningFlag();
        
    // global consistency check
    checkConsistency();
    if (mp_ExecMsgs->isErrorFlag()) return stopAppReturn();
    mp_ExecMsgs->resetWarningFlag();
        
    // simulation
    runSimulation();
    if (mp_ExecMsgs->isErrorFlag()) return stopAppReturn();
    mp_ExecMsgs->resetWarningFlag();
        
    wxTimeSpan EffSimTime = m_EffectiveEndTime.Subtract(m_EffectiveStartTime);
    m_ExSI.RunTime = EffSimTime;
    

    // saving results

    
    
    if (mp_RunEnv->isWriteResults() && !mp_ExecMsgs->isErrorFlag())
    {  
      saveResults();
      if (mp_ExecMsgs->isErrorFlag()) return stopAppReturn();
      mp_ExecMsgs->resetWarningFlag();
    }  
    
    if (mp_RunEnv->isWriteSimReport())
    {  
      saveSimulationReports();
      mp_ExecMsgs->resetWarningFlag();
    }  
    
            
    m_TotalEndTime = wxDateTime::Now();

    if (mp_RunEnv->isWriteResults() || mp_RunEnv->isWriteSimReport()) std::cout << std::endl;

    wxTimeSpan TotSimTime = m_TotalEndTime.Subtract(m_TotalStartTime);

    printlnExecMessagesStats();
    std::cout << std::endl;
    
    std::cout << "Simulation run time: " << EffSimTime.Format(wxT("%Hh %Mm %Ss")).mb_str(wxConvUTF8) << std::endl;
    std::cout << "     Total run time: " << TotSimTime.Format(wxT("%Hh %Mm %Ss")).mb_str(wxConvUTF8) << std::endl;
    std::cout << std::endl;
    
    return 0;
    
  }
  else return 127;   

}



// =====================================================================
// =====================================================================

int MHYDASApp::OnExit()
{

}


// =====================================================================
// =====================================================================





