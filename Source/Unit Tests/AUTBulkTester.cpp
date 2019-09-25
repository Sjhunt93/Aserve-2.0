//
//  AUTBulkTester.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 06/08/2019.
//

#include "AUTBulkTester.hpp"


/*static*/ String AUTBulkTester::sourceFolderPath = "/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/IAP-2018-2019-master/iapProj/Source/";
/*static*/ String AUTBulkTester::executablePath = "/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/IAP-2018-2019-master/iapProj/Builds/MacOSX/build/Debug/iapProj";
/*static*/ String AUTBulkTester::xcodeProjPath = "/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/IAP-2018-2019-master/iapProj/Builds/MacOSX/";


AUTBulkTester::AUTBulkTester (AserveComs & _coms) : coms(_coms), Thread("Bulk Test")
{
    revertOriginalSourceFiles = false;
}
AUTBulkTester::~AUTBulkTester ()
{
    
}

void AUTBulkTester::paint (Graphics &g)
{
    g.fillAll(Colours::darkgrey);
    g.setColour(Colours::white);
    g.drawText("Unit Tests", 0, 0, getWidth(), 50, Justification::centred);
}
void AUTBulkTester::resized ()
{
    
}

void AUTBulkTester::actionListenerCallback (const String& message)
{
    if (message.startsWith("finished")) {
        if (unitTest != nullptr) {
            std::cout << unitTest->getErrors() << " : Unit result: " << unitTest->getResult() << "\n";
            AserveUnitTest::eTestState state = unitTest->getResult();
     
            students[studentIndex].scores[testIndex] = unitTest->getResult();
                   unitTest = nullptr;
        }
        
        
        
        coms.reset();
        
        testIndex++;
        if (testIndex >= 10) {
            testIndex = 0;
            studentIndex++;
//            system("killall Terminal");
        }
        if (studentIndex < students.size()) {
            nextTest();
        }
        else {
            //test finished:
            int lastPass = 0;
            int lastFail = 0;
            for (Student s : students) {
                std::cout << "Score for : " << s.number << "\n";
                int pass =0;
                int fail = 0;
                for (int i = 0; i < 10; i++) {
                    if (s.scores[i] == AserveUnitTest::eEndedPass) {
                        pass++;
                    }
                    if (s.scores[i] == AserveUnitTest::eEndedFail) {
                        fail--;
                    }
                }
                lastPass = pass;
                lastFail = fail;
                std::cout << "Passed: " << pass << " || Failed: " << fail << "\n";
            }
            
            if (revertOriginalSourceFiles) {
                File codeFolder = File(sourceFolderPath);
                File cpp = codeFolder.getChildFile("_IAP.cpp");
                File h = codeFolder.getChildFile("_IAP.h");
                if (cpp.exists()) {
                    File f = codeFolder.getChildFile("IAP.cpp");
                    if (f.exists()) {
                        f.deleteFile();
                    }
                    cpp.copyFileTo(f);
                    cpp.deleteFile();
                }
                if (h.exists()) {
                    File f = codeFolder.getChildFile("IAP.h");
                    if (f.exists()) {
                        f.deleteFile();
                    }
                    h.copyFileTo(f);
                    h.deleteFile();
                }
                AlertWindow al("Results", "Your score is", AlertWindow::NoIcon);
                for (int i = 0; i < 10; i++) {
                    String text = String("Test ") + String(i+1) + String(" ") + String(AUT::unitTestNames[i]);
                    text += (students[0].scores[i] == AserveUnitTest::eEndedPass) ? String(": Pass") : String(": Fail");
                    al.addTextBlock(text);
                }
                int a = al.runModalLoop();
                
            }
        }

    }

}

void AUTBulkTester::run()
{
    while (!threadShouldExit()) {
        
    }
}

void AUTBulkTester::nextTest ()
{

    coms.reset();

    Student currentS = students[studentIndex];
    File fRoot(currentS.fRoot);
    fRoot = fRoot.getChildFile("unit test " + String(testIndex+1));
    File fCpp(fRoot.getChildFile("Pass_IAP.cpp"));
    File fH(fRoot.getChildFile("Pass_IAP.h"));
    
    std::cout << fCpp.getFullPathName() << "\n";
    std::cout << fH.getFullPathName() << "\n";
    
    if (fCpp.exists() && fH.exists()) {

        File codeFolder = File(sourceFolderPath);
//        File cpp = codeFolder.getChildFile("IAP.cpp");
//        File h = codeFolder.getChildFile("IAP.h");
        
        File cpp = codeFolder.getChildFile("IAP.cpp");
        File h = codeFolder.getChildFile("IAP.h");
        
        if (cpp.exists()) {
            cpp.deleteFile();
        }
        if (h.exists()) {
            h.deleteFile();
        }
        
        fCpp.copyFileTo(cpp);
        fH.copyFileTo(h);
        
        //delete old executable..
        File executable(executablePath);
        if (executable.exists()) {
            executable.deleteFile();
        }
        File fWd(xcodeProjPath);
        fWd.setAsCurrentWorkingDirectory();
        system("xcodebuild -scheme \"iapProj (ConsoleApp)\" build");
        
        executable.getParentDirectory().setAsCurrentWorkingDirectory(); //reset this for file path permissions..
        system("killall iapProj"); //these system calls are not ideal but since this is not distrubuted and only used for learning i guess its ok!
        system("open iapProj");
        
        
        Thread::sleep(1000);
        
        
        unitTest = AserveUnitTest::allocateForTest(AUT::unitTestNames[testIndex], coms);
        if (unitTest != nullptr) {
            
            std::cout << "Starting test: " << students[studentIndex].number << " : " << studentIndex << " : " << testIndex << "\n";
            unitTest->addActionListener(this);
            unitTest->startUnitTest(1000);
        }
    }
    else {
        students[studentIndex].testsNotAttempted++;
        actionListenerCallback("finished");
    }
}

void AUTBulkTester::runBulkTest (File fRoot)
{
    
    AserveUnitTest::projectPath = "/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/IAP-2018-2019-master/iapProj/Source/";
    
    fPath = fRoot;
    
    Array<File> files;
    fPath.findChildFiles(files, File::findDirectories, false);
    std::cout << fRoot.getFullPathName() << "\n";
    
    for (File f : files) {
        Student student;
        student.fRoot = f;
        student.number = f.getFileName();
        students.push_back(student);
        std::cout << student.number << " - " << student.fRoot.getFullPathName() << "\n";
    }
    
    studentIndex = 0;
    testIndex = 0;

    nextTest();
}

void AUTBulkTester::runFolderOfTest (File solutionsPath)
{
    
    String projectPath = AserveUnitTest::projectPath;
    sourceFolderPath = projectPath;
//    Builds/MacOSX/build/Debug/iapProj
    {
        File ex = File(projectPath).getParentDirectory().getChildFile("Builds").getChildFile("MacOSX").getChildFile("build").getChildFile("Debug").getChildFile("iapProj");
        if (ex.exists()) {
            executablePath = ex.getFullPathName();
        }
        else {
            return;
        }
    }
    {
        File proj = File(projectPath).getParentDirectory().getChildFile("Builds").getChildFile("MacOSX");
        if (proj.exists()) {
            xcodeProjPath = proj.getFullPathName();
        }
        else {
            return;
        }

    }
    system("killall iapProj"); //these system calls are not ideal but since this is not distrubuted and only used for learning i guess its ok!
    revertOriginalSourceFiles = true;
    
    {
        //copy original source files:
        
        File codeFolder = File(sourceFolderPath);
        File cpp = codeFolder.getChildFile("IAP.cpp");
        File h = codeFolder.getChildFile("IAP.h");
        
        h.copyFileTo(codeFolder.getChildFile("_IAP.h"));
        cpp.copyFileTo(codeFolder.getChildFile("_IAP.cpp"));
    }
    
    
    
    students.clear();
    Student student;
    student.fRoot = AserveUnitTest::solutionsPath;
    student.number = File(AserveUnitTest::solutionsPath).getFileName();
    students.push_back(student);
    std::cout << student.number << " - " << student.fRoot.getFullPathName() << "\n";
    
    studentIndex = 0;
    testIndex = 0;
    
    nextTest();

}
