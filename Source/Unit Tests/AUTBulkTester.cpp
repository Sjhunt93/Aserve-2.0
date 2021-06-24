//
//  AUTBulkTester.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 06/08/2019.
//

#include "AUTBulkTester.hpp"



///------------------------------------------------------------------------------------------
// the path you need to change!
#warning CHANGE ME :D
/*static*/ String AUTBulkTester::aserveBulkTesterPath = "/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/IAP-2018-2019-master/";

/*static*/ String AUTBulkTester::staffMarkingSolutionsFolder = "/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/Submissions/";

// if you only want to run the first 6 tests then change the line of code below...
/*static*/ int AUTBulkTester::numberOfTestsToRun = 3;




///------------------------------------------------------------------------------------------


// Path of the source files to copy and compile.
/*static*/ String AUTBulkTester::sourceFolderPath = aserveBulkTesterPath + "/iapProj/Source/";

//run this to test the students work..
/*static*/ String AUTBulkTester::executablePath = aserveBulkTesterPath + "/iapProj/Builds/MacOSX/build/Debug/iapProj";
// where is the xcode proj folder..
/*static*/ String AUTBulkTester::xcodeProjPath = aserveBulkTesterPath + "/iapProj/Builds/MacOSX/";


AUTBulkTester::AUTBulkTester (AserveComs & _coms) : Thread("Bulk Test"), coms(_coms)
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
        
            students[studentIndex].scores[testIndex] = unitTest->getResult();
                   unitTest = nullptr;
        }
        coms.reset();
        
        testIndex++;

        if (testIndex >= numberOfTestsToRun) { //
            testIndex = 0;
            studentIndex++;
        }
        if (studentIndex < students.size()) {
            nextTest();
        }
        else { // at this point all tests are done...
            
            { //end the test
                File executable(executablePath);
                executable.getParentDirectory().setAsCurrentWorkingDirectory(); //reset this for file path permissions..
                system("killall iapProj"); //these system calls are not ideal but since this is not distrubuted and only used for learning i guess its ok!
            }
            
            ///-------------------------------------------------------------------------------------
            //Printing
            ///-------------------------------------------------------------------------------------
            printAllResultsStaff();
         
            ///-------------------------------------------------------------------------------------
            // for a single student testing their work!
            ///-------------------------------------------------------------------------------------
            restoreStudentIAPFiles();
            
        }

    }

}

void AUTBulkTester::printAllResultsStaff ()
{
    std::cout << "PRINTING REULTS IN CSV\n\n\n";
    std::cout << "PRINT KEY: 0 = no test, 2 = pass, 3 = fail (or timeout).";
    std::cout << "PRINT HEADER\n";
    std::cout << "FolderName, Name, Email, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 \n";
    int lastPass = 0;
    int lastFail = 0;
    
    for (Student s : students) {
        int pass = 0;
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
        //                std::cout << "Passed: " << pass << " || Failed: " << fail << "\n";
        std::cout << s.number << "," << s.name << "," << s.email << "," ;// << pass << ",";
        for (int i = 0; i < 10; i++) {
            std::cout << s.scores[i] << ",";
        }
        std::cout << "\n";
        
    }
}
void AUTBulkTester::restoreStudentIAPFiles ()
{
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

void AUTBulkTester::run()
{
    while (!threadShouldExit()) {
        
    }
}

void AUTBulkTester::nextTest ()
{

    coms.reset();

    Student currentS = students[studentIndex];
    File fRoot(currentS.fRoot); //all 10 test..
    fRoot = fRoot.getChildFile("unit test " + String(testIndex+1));
    File fCpp(fRoot.getChildFile("Pass_IAP.cpp"));
    File fH(fRoot.getChildFile("Pass_IAP.h"));
    
    std::cout << fCpp.getFullPathName() << "\n";
    std::cout << fH.getFullPathName() << "\n";
    
    
    // If for any reason files are not in folders then this tries another method based on windows file paths.
    if (!fCpp.exists()) {
        File fPath(currentS.fRoot);
        fCpp = fPath.getChildFile("unit test " + String(testIndex+1) + String("\\Pass_IAP.cpp")); //attempt 2
    }
    if (!fH.exists()) {
        File fPath(currentS.fRoot);
        fH = fPath.getChildFile("unit test " + String(testIndex+1) + String("\\Pass_IAP.h")); //attempt 2
    }
    
    
    if (fCpp.exists() && fH.exists()) {

        File codeFolder = File(sourceFolderPath);
        
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
        
        //compile the project
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

// run full marking test suite..
void AUTBulkTester::runBulkTestStaff (File fRoot)
{
    
    AserveUnitTest::projectPath = AUTBulkTester::sourceFolderPath;
    
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
        
        File hiddenFile = f.getChildFile(".sub.txt");
        
        student.hasHiddenFile = hiddenFile.exists();
//        if (!hiddenFile.exists()) {
//            continue;
//
//        }
        
        FileInputStream fStream(hiddenFile);
        
        String logonName = fStream.readString();
        String getFullUserName = fStream.readString();
        String CPUName = fStream.readString();
        String sId = fStream.readString();
        String email = fStream.readString();
        
        students[students.size()-1].name = getFullUserName.removeCharacters("\n");
        students[students.size()-1].email = email.removeCharacters("\n");
        
    }
    
    studentIndex = 0;
    testIndex = 0;

    nextTest();
}

void AUTBulkTester::runFolderOfTestStudent (File solutionsPath)
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
    student.fRoot = AserveUnitTest::solutionsPath; //sent via OSC from IAP proj
    student.number = File(AserveUnitTest::solutionsPath).getFileName();
    students.push_back(student);
    std::cout << student.number << " - " << student.fRoot.getFullPathName() << "\n";
    
    studentIndex = 0;
    testIndex = 0;
    
    nextTest();

}
