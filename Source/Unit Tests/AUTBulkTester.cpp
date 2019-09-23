//
//  AUTBulkTester.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 06/08/2019.
//

#include "AUTBulkTester.hpp"



AUTBulkTester::AUTBulkTester (AserveComs & _coms) : coms(_coms), Thread("Bulk Test")
{

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
                std::cout << "Passed: " << pass << " || Failed: " << fail << "\n";
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

        File codeFolder = File("/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/IAP-2018-2019-master/iapProj/Source/");
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
        File executable("/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/IAP-2018-2019-master/iapProj/Builds/MacOSX/build/Debug/iapProj");
        if (executable.exists()) {
            executable.deleteFile();
        }
        File fWd("/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/IAP-2018-2019-master/iapProj/Builds/MacOSX/");
        fWd.setAsCurrentWorkingDirectory();
        system("xcodebuild -scheme \"iapProj (ConsoleApp)\" build");
        
        executable.getParentDirectory().setAsCurrentWorkingDirectory(); //reset this for file path permissions..
        system("killall iapProj");
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
