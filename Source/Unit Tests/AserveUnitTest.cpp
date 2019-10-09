//
//  AserveUnitTest.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 07/03/2019.
//

#include "AserveUnitTest.hpp"
#include "AUTMtof.hpp"
#include "AUTDrumSampler.hpp"
#include "AUTTestTone.hpp"
#include "AUChordMachine.hpp"
#include "AUTNoteoff.hpp"
#include "AUTScaleQuantiser.hpp"
#include "AUTHighFreqTest.hpp"
#include "AUTVectorSequencer.hpp"
#include "AUTFileRead.hpp"
#include "AUFileWrite.hpp"
#include "AUTAlgorithms.hpp"

const String AserveUnitTest::TEST_TIMEOUT = "Test Timeout - please check your code for excessive sleeps.\n";

#ifdef DEBUG
String AserveUnitTest::solutionsPath = "";// "~/Desktop/IAP-2019-2020-master/Solutions/Unit Tests";
String AserveUnitTest::projectPath = "";// "~/Desktop/IAP-2019-2020-master/iapProj/Source";
#else
    String AserveUnitTest::solutionsPath = "";
    String AserveUnitTest::projectPath = "";
#endif

AserveUnitTest::AserveUnitTest (AserveComs & _coms, String unitTestName, String fn) : Thread("Unit Tester"), coms(_coms), testName(unitTestName), folderName(fn), randomNumber(Time::currentTimeMillis())
{
    timeout = 0;
    currentState = eNone;
    defaultTimeOutInc = 100;
    coms.setUniTestPtr(this);
}

AserveUnitTest::~AserveUnitTest ()
{
    coms.setUniTestPtr(nullptr);
}

//Thread will call this, then call runningUnitTest

void AserveUnitTest::startUnitTest (const int to)
{
    timeout = to;
    errorMessages = "";
    simpleMutex.set(0);
    messageQue.clear();
    
    currentState = eRunning;
    startThread(3); //lowish priority.
    
}
void AserveUnitTest::run()
{
    while (!threadShouldExit() && timeout > 0 && currentState == eRunning) {
        runningUnitTest();
        Thread::sleep(defaultTimeOutInc);
        timeout -= defaultTimeOutInc;
    }
    if (timeout < 0) {
        errorMessages += TEST_TIMEOUT;
    }
    sendActionMessage("finished");

}

void AserveUnitTest::testMessageReceived (String message, std::vector<String> params)

//void AserveUnitTest::testOscMessageReceived (const OSCMessage& message)
{
    
    String decodedMsg = "";
    
    decodedMsg = message;
    for (String v : params) {
        decodedMsg += "|" + String(v);
    }
    
    /*
     Decode message then lock the thread and add  message to que so running thread can pull out info...
     */
    
    while (simpleMutex.get()) { //will keep trying while message is locked by other operation
        
    }
    simpleMutex.set(true);
    messageQue.add(decodedMsg);
    simpleMutex.set(false);
}

StringArray AserveUnitTest::getAndClearMessageLog ()
{
    while (simpleMutex.get()) { //will keep trying while message is locked by other operation
        
    }
    simpleMutex.set(true);
    StringArray messages = messageQue;
    messageQue.clear();
    
    
    simpleMutex.set(false);
    return messages;
}

StringArray AserveUnitTest::getCue (int timeout, int messagesToWaitFor, const int lagInMs)
{
    
    StringArray tt;
    
    if (threadShouldExit()) {
        return tt;
    }
    
    int counter = 0;
    const bool expectsMessages = messagesToWaitFor > 0;
    
    auto ending = [&]() -> bool {
        if (expectsMessages) {
            return tt.size() < messagesToWaitFor && counter < timeout;
        }
        else {
            return counter < timeout;
        }
    };
    
    do {
        tt.addArray(getAndClearMessageLog());
        Thread::sleep(lagInMs);
        counter += lagInMs;
    } while (ending() && !threadShouldExit());
    
    
    
    
    if (!tt.size() && expectsMessages) { //if no messages are expected then this test should simply timeout..
        errorMessages += "No messages received test timed out!\n";
    }
    
    return tt;
}

AserveUnitTest::eTestState AserveUnitTest::getResult ()
{
    return currentState;
}

String AserveUnitTest::getErrors ()
{
    return errorMessages;
}

 File AserveUnitTest::fodlerForTestResults (String name)
{
    File root(solutionsPath);
    
    StringArray list = getTestList();
    const int index  = list.indexOf(name);
    if (index >= 0) {
        String name = "unit test " + String(index+1);
		if (root.exists()) {
			File fFolder = root.getChildFile(name);
			return fFolder;
		}
    }
    
    return File("");
}

void AserveUnitTest::saveToFile ()
{
    File root(solutionsPath);
	if (!root.exists()) {
		return;
	}
    File fFolder = root.getChildFile(folderName);

    // do not mess with directory structure on disk
    // the IAP github repository should contain all required directory stubs
    if( File(solutionsPath).exists() == false )
    {
        String msg = "Invalid unit test solutions path ";
               msg += solutionsPath;
        coms.addUnitTestMessageToLog(msg);
    }
  
    if( File(projectPath).exists() == false )
    {
        String msg = "Invalid unit test project path ";
               msg += projectPath;
        coms.addUnitTestMessageToLog(msg);
    }
  
    if ( fFolder.exists() && fFolder.isDirectory() ) {
      
        File codeFolder = File(projectPath);
        File cpp = codeFolder.getChildFile("IAP.cpp");
        File h = codeFolder.getChildFile("IAP.h");
        if (cpp.exists() && h.exists()) {
            
            String prePend = (getResult() == eEndedPass) ? "Pass_" : "Failed_";
            
            File hCopy = fFolder.getChildFile(prePend + "IAP.h");
            File cppCopy = fFolder.getChildFile(prePend + "IAP.cpp");
            if (hCopy.exists()) {
                hCopy.deleteFile();
            }
            if (cppCopy.exists()) {
                cppCopy.deleteFile();
            }
            h.copyFileTo(hCopy);
            cpp.copyFileTo(cppCopy);
            
            waterMarkFile2(hCopy);
            waterMarkFile2(cppCopy);
            createHiddenFile(hCopy.getParentDirectory());
        }
    }
}


/*static*/ StringArray AserveUnitTest::getTestList ()
{
    StringArray array;
    for (int i = 0; i < AUT::nbrOfTests; i++) {
        array.add(AUT::unitTestNames[i]);
    }
    return array;
}

AserveUnitTest * AserveUnitTest::allocateForTest (String t, AserveComs & coms)
{
    StringArray list = getTestList();
    const int index  = list.indexOf(t);
    switch (index) {
        case 0:
            return new AUTMtof(coms);
        case 1:
            return new AUTDrumSampler(coms);
        case 2:
            return new AUTChordMachine(coms);
        case 3:
            return new AUTNoteoff(coms);
        case 4:
            return new AUTScaleQuantiser(coms);
        case 5:
            return new AUTHighFreqTest(coms);
        case 6:
            return new AUTVectorSequencer(coms);
        case 7:
            return new AUTFileRead(coms);
        case 8:
            return new AUTFileWrite(coms);
        case 9:
            return new AUTAlgorithms(coms);
        default:
            break;
    }
    return nullptr;
}

/*static*/ AserveUnitTest::eTestState AserveUnitTest::getStateForTest (String t)
{
    File f = fodlerForTestResults(t);
    if (f.exists() && f.isDirectory()) {
        if (f.getChildFile("Pass_IAP.cpp").exists()) {
            return eTestState::eEndedPass;
        }
        else if (f.getChildFile("Failed_IAP.cpp").exists()) {
            return eTestState::eEndedFail;
            
        }
    }
    return eTestState::eNone;
}


/*static*/ void AserveUnitTest::waterMarkFile2 (File f)
{
    String all;
    {
        FileInputStream fStream(f);
        
        bool gate = false;
        while (!fStream.isExhausted()) {
            String s = fStream.readNextLine();
            if (s.contains("#")) {
                gate = true;
            }
            if (gate) {
                all += s + "\n";
            }
            
        }
    }
    

    
    String usrName = SystemStats::getLogonName().removeCharacters(" ");
    uint8 counter = 0;
    for (int i = 0; i < usrName.length(); i++) {
        char c = usrName.toRawUTF8()[i];
        
        for (int j = 0; j < 8; j++) {
            if ((1 << j) & c) {
                counter++;
            }
            
        }
    }
    
    String checkSum = "";
    for (int i = 0; i < 8; i++) {
        if ((1 << i) & counter) {
            checkSum += " ";
            
        }
        else {
            checkSum += "\t";
        }
    }
    
    FileOutputStream outStream(f);
    
    outStream.setPosition(0);
    outStream.truncate();
    
    outStream << String("// Unit Test copy: DO NOT EDIT THIS HEADER \n");
    outStream << String("// " + f.getFileName() + "\n");
    outStream << String("// IAPProject \n");
    outStream << String("//" + checkSum + "\n");
    outStream << String("// Created by " + SystemStats::getLogonName() + "\n");
    outStream << String("// Created by " + SystemStats::getFullUserName() + "\n");
    outStream << String("// Tested On " + Time::getCurrentTime().toString(true, true) + "\n");
    outStream << String("// End of Header;\n\n\n");

    outStream << all;

}

/*static*/ void AserveUnitTest::createHiddenFile (File dir)
{
    File f = dir.getChildFile(".code.txt");
    if (f.exists()) {
        f.deleteFile();
    }
    FileOutputStream fStream(f);
    
    fStream.writeString(SystemStats::getLogonName() + "\n");
    fStream.writeString(SystemStats::getFullUserName() + "\n");
    fStream.writeString(SystemStats::getComputerName() + "\n");
    fStream.writeString(Time::getCurrentTime().toString(true, true) + "\n");
}

/*static*/ void AserveUnitTest::prepareSubmission (String sId, String email)
{
    File solPath(AserveUnitTest::solutionsPath);
    //Source https://forum.juce.com/t/error-creating-zip-file-from-folder-w-subdirectories/10796/5
    
    {
        File f = solPath.getChildFile(".sub.txt");
        if (f.exists()) {
            f.deleteFile();
        }
        FileOutputStream fStream(f);
        fStream.writeString(SystemStats::getLogonName() + "\n");
        fStream.writeString(SystemStats::getFullUserName() + "\n");
        fStream.writeString(SystemStats::getComputerName() + "\n");
        fStream.writeString(sId + "\n");
        fStream.writeString(email + "\n");
        fStream.writeString(Time::getCurrentTime().toString(true, true) + "\n");
    }
    
    
    ZipFile::Builder zipBuilder;
    Array<File> tempFiles;
    solPath.findChildFiles(tempFiles, File::findFiles, true, "*");
    
    for (int i = 0; i < tempFiles.size(); i++)
    {
        zipBuilder.addFile(tempFiles[i], 0, tempFiles[i].getRelativePathFrom(solPath));
    }
    
    String pathName = sId + "_submission.zip";
    //save our zip file
    double *progress = nullptr;
    
    File outputZip = solPath.getParentDirectory().getChildFile(pathName);
    if (outputZip.exists()) {
        outputZip.deleteFile();
    }
    std::cout << outputZip.getFullPathName();
    FileOutputStream os (outputZip);
    zipBuilder.writeToStream(os, progress);
}

int AserveUnitTest::arc4random()
{
	return randomNumber.nextInt();
}