/**
  \file ColFileParser.cpp
  \brief implements column file parser

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "ColTextParser.h"
#include <wx/tokenzr.h>

#include <iostream>


// =====================================================================
// =====================================================================



ColumnTextParser::ColumnTextParser(wxString CommentLineSymbol, wxString Delimiter)
{

  m_Delimiter = Delimiter;
  m_CommentSymbol = CommentLineSymbol;

  mp_Contents = NULL;

  m_LinesCount = 0;
  m_ColsCount = 0;

}



// =====================================================================
// =====================================================================



ColumnTextParser::~ColumnTextParser()
{

}


// =====================================================================
// =====================================================================



wxArrayString* ColumnTextParser::tokenizeLine(wxString Line)
{
  wxArrayString *NewLine = new wxArrayString();

  wxStringTokenizer Tokenizer(Line, m_Delimiter);

  while (Tokenizer.HasMoreTokens())
  {
    wxString Token = Tokenizer.GetNextToken();
    NewLine->Add(Token);
  }


  return NewLine;
}


// =====================================================================
// =====================================================================



bool ColumnTextParser::checkContents()
{
  int LineColCount;
  int LineCount = mp_Contents->GetCount();

  if (LineCount == 0) return true;

  // checks that all lines have the same size
  // i.e. same columns number
  LineColCount = ((wxArrayString*)(mp_Contents->Item(0)))->GetCount();

  for (int i=1;i<LineCount;i++)
  {
    if (((wxArrayString*)(mp_Contents->Item(i)))->GetCount() != LineColCount)
      return false;
  }

  m_LinesCount = mp_Contents->GetCount();
  m_ColsCount = LineColCount;

  return true;
}


// =====================================================================
// =====================================================================


bool ColumnTextParser::isCommentLineStr(wxString LineStr)
{

  if (m_CommentSymbol.Length() > 0)
  {
    LineStr = LineStr.Trim(false);
    if (LineStr.StartsWith(m_CommentSymbol.c_str()))
      return true;
  }

  return false;

}



// =====================================================================
// =====================================================================


bool ColumnTextParser::isEmptyLineStr(wxString LineStr)
{

  LineStr = LineStr.Trim(true).Trim(false);

  if (LineStr.Length() == 0) return true;
  else return false;

}



// =====================================================================
// =====================================================================



bool ColumnTextParser::loadFromFile(wxString Filename)
{

  bool IsOK = true;
  int LinesCount;
  wxString StrLine;
  wxTextFile *ColumnFile;

  if (mp_Contents != NULL) delete mp_Contents;
  mp_Contents = new ArrayContents();

  m_LinesCount = 0;
  m_ColsCount = 0;


  ColumnFile = new wxTextFile(Filename);

  // check if file exists and opens it
  if (!ColumnFile->Exists()) return false;
  if (!ColumnFile->Open()) return false;

  LinesCount = ColumnFile->GetLineCount();

  if (LinesCount == 0) return false;

  // identifies file type, i.e. DOS/MAC/UNIX/...
  ColumnFile->GuessType();

  // parse and loads file contents
  StrLine = ColumnFile->GetFirstLine();

  while (!ColumnFile->Eof())
  {
    // adds the line if it is not a comment line nor empty
    if (!isCommentLineStr(StrLine) && !isEmptyLineStr(StrLine)) mp_Contents->Add(tokenizeLine(StrLine));

    StrLine = ColumnFile->GetNextLine();
  }

  ColumnFile->Close();

  delete ColumnFile;


  return checkContents();

}


// =====================================================================
// =====================================================================

bool ColumnTextParser::setFromString(wxString Contents, int ColumnsNbr)
{
  return false;
}

// =====================================================================
// =====================================================================



wxArrayString* ColumnTextParser::getValues(int Line)
{
  if (Line < mp_Contents->GetCount())
  {
    return ((wxArrayString*)(mp_Contents->Item(Line)));
  }
  else
  {
    return NULL;
  }

}



// =====================================================================
// =====================================================================


wxString ColumnTextParser::getValue(int Line, int Column)
{
  wxArrayString* LineString = getValues(Line);

  if (LineString != NULL && Column < LineString->GetCount())
  {
    return LineString->Item(Column);
  }
  else
  {
    return wxT("");
  }

}



// =====================================================================
// =====================================================================


bool ColumnTextParser::getStringValue(int Line, int Column, wxString *Value)
{
  wxString StrValue = getValue(Line,Column);

  if (StrValue.Length() == 0) return false;

  *Value = StrValue;

  return true;

}


// =====================================================================
// =====================================================================


bool ColumnTextParser::getLongValue(int Line, int Column, long* Value)
{
  wxString StrValue = getValue(Line,Column);

  if (StrValue.Length() != 0)
  {
    return StrValue.ToLong(Value);
  }

  return false;
}



// =====================================================================
// =====================================================================


bool ColumnTextParser::getDoubleValue(int Line, int Column, double* Value)
{

  wxString StrValue = getValue(Line,Column);

  if (StrValue.Length() != 0)
  {
    return StrValue.ToDouble(Value);
  }

  return false;
}


