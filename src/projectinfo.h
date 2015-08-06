#pragma once

#include <string>
#include <vector>

class ProjectInfo {
public:
  enum Subsystem {
    Console,
    Gui,
    StaticLib,
    SharedLib
  };

  ProjectInfo(const std::string& vcFileContents);
  std::string                     GetAsQtProject() const;
  bool                            IsOk() const          { return mIsOk; }
  const std::string&              GetName() const       { return mName; }
  Subsystem                       GetSubsystem() const  { return mSubsystem; }
  const std::vector<std::string>& GetHeaders() const    { return mHeaders; }
  const std::vector<std::string>& GetSources() const    { return mSources; }
  const std::vector<std::string>& GetDefines() const    { return mDefines; }
  const std::vector<std::string>& GetLibDirs() const    { return mLibDirs; }
  const std::vector<std::string>& GetLibs() const       { return mLibs; }
private:
  bool                      mIsOk;
  std::string               mName;
  Subsystem                 mSubsystem;
  std::vector<std::string>  mHeaders;
  std::vector<std::string>  mSources;
  std::vector<std::string>  mDefines;
  std::vector<std::string>  mLibDirs;
  std::vector<std::string>  mLibs;
};
