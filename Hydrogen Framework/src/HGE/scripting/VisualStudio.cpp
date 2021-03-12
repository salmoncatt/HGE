#include "hpch.h"
#include "VisualStudio.h"

namespace HGE {

	std::string VisualStudio::hFile = 
R"(#include "hpch.h"
#include "HGE.h"

namespace %s{

	class %s : public Script {
	public:
		void start();
		void update();
		void fixedUpdate();
		void lateUpdate();
		void stop();
	};
	RegisterScript(%s)

})";
	std::string VisualStudio::cppFile = 
R"(#include "%s"
using namespace HGE;

namespace %s{

	void %s::start() {

	}

	void %s::update() {

	}

	void %s::fixedUpdate() {

	}

	void %s::lateUpdate() {

	}

	void %s::stop() {

	}

})";


	void VisualStudio::openVisualStudio(const std::string& solutionPath) {
		ShellExecute(NULL, "open", solutionPath.c_str(), NULL, NULL, SW_SHOW);
	}

	void VisualStudio::addScriptToProject(const std::string& projectPath, const std::string& namespaceName, const std::string& scriptName) {
		std::string projectData = Util::readAsString(projectPath, HGE_NORMAL_READ);
		std::string formatedHfile;
		std::string formatedCPPfile;

		char* cppFileBuffer = new char[cppFile.length() * 2];
		char* hFileBuffer = new char[hFile.length() * 2];

		sprintf_s(hFileBuffer, cppFile.length() * 2, hFile.c_str(), namespaceName.c_str(), scriptName.c_str(), scriptName.c_str());
		sprintf_s(cppFileBuffer, cppFile.length() * 2, cppFile.c_str(), (scriptName + ".h").c_str(), namespaceName.c_str(), scriptName.c_str(), scriptName.c_str(), scriptName.c_str(), scriptName.c_str(), scriptName.c_str());

		formatedHfile = hFileBuffer;
		formatedCPPfile = cppFileBuffer;

		delete[] cppFileBuffer;
		delete[] hFileBuffer;

		//start half way to find the last </Project> so i can insert the code into there
		size_t index = projectData.find("</Project>", projectData.length() / 2);
		
		std::string projectIncludeData = 
R"(
<ItemGroup>
<ClInclude Include="src\)" + scriptName + R"(.h" />)" + 
R"(
</ItemGroup>

<ItemGroup>
<ClCompile Include="src\)" + scriptName + R"(.cpp" />)" + 
R"(
</ItemGroup>

)";
		projectData.insert(index, projectIncludeData);

		Debug::setSystemLogMode(HGE_DONT_LOG_ON_SUCCESS, HGE_LOG_ON_FAIL);
		Util::writeAsString(formatedHfile, Util::removeNameFromFilePathAndName(projectPath) + R"(src\)" + scriptName + ".h", HGE_NORMAL_WRITE);
		Util::writeAsString(formatedCPPfile, Util::removeNameFromFilePathAndName(projectPath) + R"(src\)" + scriptName + ".cpp", HGE_NORMAL_WRITE);
		Util::writeAsString(projectData, projectPath, HGE_NORMAL_WRITE);
		Debug::setSystemLogMode(HGE_LOG_ON_SUCCESS, HGE_LOG_ON_FAIL);

	}

}