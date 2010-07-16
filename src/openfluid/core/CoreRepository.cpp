/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


#include <openfluid/core/CoreRepository.hpp>


namespace openfluid { namespace core {



CoreRepository* CoreRepository::mp_Singleton = NULL;

// =====================================================================
// =====================================================================


struct SortUnitsPtrByProcessOrder
{
  bool operator ()(Unit*& U1,Unit*& U2) const
  {
    return (U1->getProcessOrder() <= U2->getProcessOrder());
  }

};

// =====================================================================
// =====================================================================

CoreRepository::CoreRepository()
{
//  m_LastMemToDiskStep = -1;
}


// =====================================================================
// =====================================================================


CoreRepository* CoreRepository::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new CoreRepository();
  return mp_Singleton;
}




// =====================================================================
// =====================================================================


bool CoreRepository::addUnit(Unit aUnit)
{
  Unit* TheUnit = m_PcsOrderedUnitsByClass[aUnit.getClass()].addUnit(aUnit);
  if (TheUnit != NULL)
  {
    m_PcsOrderedUnitsGlobal.push_back(TheUnit);
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool CoreRepository::isUnitsClassExist(UnitClass_t UnitClass) const
{
  return m_PcsOrderedUnitsByClass.find(UnitClass) != m_PcsOrderedUnitsByClass.end();
}

// =====================================================================
// =====================================================================


Unit* CoreRepository::getUnit(UnitClass_t UnitClass, UnitID_t UnitID)
{
  UnitsListByClassMap_t::iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitClass);

  if (it != m_PcsOrderedUnitsByClass.end())
  {
    return (it->second.getUnit(UnitID));
  }

  return NULL;
}


// =====================================================================
// =====================================================================


UnitsCollection* CoreRepository::getUnits(UnitClass_t UnitClass)
{
  UnitsListByClassMap_t::iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitClass);

  if (it == m_PcsOrderedUnitsByClass.end()) return NULL;
  return  &(it->second);

}

// =====================================================================
// =====================================================================


const UnitsCollection* CoreRepository::getUnits(UnitClass_t UnitClass) const
{
  UnitsListByClassMap_t::const_iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitClass);

  if (it == m_PcsOrderedUnitsByClass.end()) return NULL;
  return  &(it->second);

}


// =====================================================================
// =====================================================================


bool CoreRepository::sortUnitsByProcessOrder()
{
  UnitsListByClassMap_t::iterator it;
  UnitsCollection* Units;

  // sort primary units structures
  for (it = m_PcsOrderedUnitsByClass.begin();it != m_PcsOrderedUnitsByClass.end();++it)
  {
    Units = &(it->second);
    Units->sortByProcessOrder();
  }

  // sort global units structure
  m_PcsOrderedUnitsGlobal.sort(SortUnitsPtrByProcessOrder());

  return true;
}

// =====================================================================
// =====================================================================


void CoreRepository::streamContents(std::ostream& OStream)
{
  UnitsListByClassMap_t::iterator ClassIt;

  UnitsList_t* Units;
  UnitsList_t::iterator UnitIt;


  if (m_PcsOrderedUnitsByClass.size() == 0)
  {
    OStream << "No unit" << std::endl;
    return;
  }

  for (ClassIt = m_PcsOrderedUnitsByClass.begin();ClassIt != m_PcsOrderedUnitsByClass.end();++ClassIt)
  {
    OStream << "** Units class : " << ClassIt->first << " **" << std::endl;


    Units = (ClassIt->second).getList();

    for (UnitIt = Units->begin();UnitIt != Units->end();++UnitIt)
    {
      UnitIt->streamContents(OStream);
    }


  }
  OStream << std::endl;
}


} } // namespaces

