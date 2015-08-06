#define RAPIDXML_NO_EXCEPTIONS
#include "projectinfo.h"
#include "str.h"
#include "../lib/rapidxml.hpp"
#include <cstdlib>
#include <iostream>

using namespace rapidxml;
using namespace std;

ProjectInfo::ProjectInfo(const std::string& vcFileContents) {
  // Object has not been initialized yet
  mIsOk = false;
  mSubsystem = Console;

  // Parse XML VC project
  xml_document<> doc;
  doc.parse<0>(const_cast<char*>(vcFileContents.c_str()));

  // Get header info
  xml_node<>* projectNode = doc.first_node("Project");
  if ( !projectNode ) return;

  // Parse project name
  xml_node<>* propertyGroupNode = projectNode->first_node("PropertyGroup");
  while ( propertyGroupNode ) {
    xml_attribute<>* labelAttr = propertyGroupNode->first_attribute("Label");

    // Parse project name
    if ( labelAttr && string(labelAttr->value()) == "Globals" ) {
      xml_node<>* rootNamespaceNode = propertyGroupNode->first_node("RootNamespace");
      if ( rootNamespaceNode ) {
        mName = rootNamespaceNode->value();
      }
    }

    propertyGroupNode = propertyGroupNode->next_sibling();
  }

  // Parse header and source files
  xml_node<>* itemGroupNode = projectNode->first_node("ItemGroup");
  while ( itemGroupNode ) {
    // Parse header files
    xml_node<>* includeNode = itemGroupNode->first_node("ClInclude");
    while ( includeNode ) {
      xml_attribute<>* attr = includeNode->first_attribute("Include");
      if ( attr ) mHeaders.push_back(str::replace(attr->value(), "\\", "/"));
      includeNode = includeNode->next_sibling();
    }

    // Parse source files
    xml_node<>* compileNode = itemGroupNode->first_node("ClCompile");
    while ( compileNode ) {
      xml_attribute<>* attr = compileNode->first_attribute("Include");
      if ( attr ) mSources.push_back(str::replace(attr->value(), "\\", "/"));
      compileNode = compileNode->next_sibling();
    }

    itemGroupNode = itemGroupNode->next_sibling();
  }

  // Parse other config
  xml_node<>* itemDefinitionGroupNode = projectNode->first_node("ItemDefinitionGroup");
  while ( itemDefinitionGroupNode ) {
    // Only parse Release config by now...
    xml_attribute<>* conditionAttr = itemDefinitionGroupNode->first_attribute("Condition");
    if ( conditionAttr && string(conditionAttr->value()).find("$(Platform)'=='Release|Win") != string::npos ) {
      // Compile config
      xml_node<>* compileNode = itemDefinitionGroupNode->first_node("ClCompile");
      if ( compileNode ) {
        // Preprocessor definitions
        xml_node<>* preprocessorNode = compileNode->first_node("PreprocessorDefinitions");
        if ( preprocessorNode ) {
          vector<string> tempDefines = str::split(preprocessorNode->value(), ';');
          for ( const string& define : tempDefines ) {
            if ( define != "%(PreprocessorDefinitions)" ) {
              mDefines.push_back(define);
            }
          }
        }
      }

      // Linker config
      xml_node<>* linkNode = itemDefinitionGroupNode->first_node("Link");
      if ( linkNode ) {
        // Subsystem
        xml_node<>* subsystemNode = linkNode->first_node("SubSystem");
        if ( subsystemNode ) {
          string subsystem = subsystemNode->value();
          if ( subsystem == "Console" ) {
            mSubsystem = Console;
          } else if ( subsystem == "Windows" ) {
            mSubsystem = Gui;
          }
        }

        // Library directories
        xml_node<>* libDirsNode = linkNode->first_node("AdditionalLibraryDirectories");
        if ( libDirsNode ) {
          mLibDirs = str::split(str::replace(libDirsNode->value(), "\\", "/"), ';');
        }

        // Libraries
        xml_node<>* depsNode = linkNode->first_node("AdditionalDependencies");
        if ( depsNode ) {
          vector<string> tempDeps = str::split(depsNode->value(), ';');
          for ( const string& lib : tempDeps ) {
            if ( lib != "%(AdditionalDependencies)" ) {
              mLibs.push_back(str::strip_ext(lib));
            }
          }
        }
      }
    }

    itemDefinitionGroupNode = itemDefinitionGroupNode->next_sibling();
  }

  // Object is initialized
  mIsOk = true;
}

string ProjectInfo::GetAsQtProject() const {
  string qtpro;

  // Write target
  qtpro += "TARGET = " + mName + "\n";

  // Write template
  switch ( mSubsystem ) {
  case Console:
    qtpro += "TEMPLATE = app\n";
    break;
  case Gui:
    qtpro += "TEMPLATE = app\n";
    break;
  }

  // Write config
  qtpro += "CONFIG -= qt\n\n";

  // Write defines
  if ( mDefines.size() > 0 ) {
    qtpro += "DEFINES +=";
    for ( const string& def : mDefines ) {
      qtpro += " " + def;
    }
    qtpro += "\n\n";
  }

  // Write lib dirs
  if ( mLibDirs.size() > 0 ) {
    qtpro += "LIBS +=";
    for ( const string& dir : mLibDirs ) {
      qtpro += " -L" + dir;
    }
    qtpro += "\n";
  }

  // Write libs
  if ( mLibs.size() > 0 ) {
    qtpro += "LIBS +=";
    for ( const string& lib : mLibs ) {
      qtpro += " -l" + lib;
    }
    qtpro += "\n\n";
  } else {
    if ( mLibDirs.size() > 0 ) qtpro += "\n";
  }

  // Write sources
  if ( mSources.size() > 0 ) {
    qtpro += "SOURCES +=";
    for ( const string& source : mSources ) {
      qtpro += " \\\n    " + source;
    }
    qtpro += "\n\n";
  }

  // Write headers
  if ( mHeaders.size() > 0 ) {
    qtpro += "HEADERS +=";
    for ( const string& header : mHeaders ) {
      qtpro += " \\\n    " + header;
    }
    qtpro += "\n\n";
  }

  return qtpro;
}
