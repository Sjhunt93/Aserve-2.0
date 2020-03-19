//
//  TheftChecker.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 03/12/2019.
//

#ifndef TheftChecker_hpp
#define TheftChecker_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AserveUnitTest.hpp"

class TheftChecker {
public:
    static void doTest ()
    {
    File fPath("/Users/sj4-hunt/Documents/Code/Aserve Unit Test Executer/Submissions/");
        
        Array<File> files;
        fPath.findChildFiles(files, File::findDirectories, false);

        
        
        
        for (File folder : files) {
            File hiddenFile = folder.getChildFile(".sub.txt");
            
            
            if (!hiddenFile.exists()) {
                std::cout << "Unable to test..." << folder.getFullPathName() << "\n";
                continue;
            
            }
            
            FileInputStream fStream(hiddenFile);
            
            String logonName = fStream.readNextLine();
            String getFullUserName = fStream.readNextLine();
            String CPUName = fStream.readNextLine();
            String sId = fStream.readNextLine();
            String email = fStream.readNextLine();
            
            std::cout << logonName << "\n";
            
            for (int i = 1; i < 11; i++) {
                File fTest = folder.getChildFile("unit test " + String(i));
                File fCpp(fTest.getChildFile("Pass_IAP.cpp"));
                File fH(fTest.getChildFile("Pass_IAP.h"));
                if (fCpp.exists()) {
                    File createdFile = fTest.getChildFile(".code.txt");
                    FileInputStream fStream2(createdFile);
                    if (!createdFile.exists()) { //!fStream2.getTotalLength()
                        std::cout << "Cannot TEST " << "\n";

                        continue;
                    }
                    String logonName2 = fStream2.readNextLine();
                    fStream2.readNextLine();
                    String getFullUserName2 = fStream2.readNextLine();
                    String CPUName2 = fStream2.readNextLine();
                    
                    FileInputStream cppStream(fCpp);
                    String a = cppStream.readNextLine();
                    String b = cppStream.readNextLine();
                    String c = cppStream.readNextLine();
                    String checkSum = cppStream.readNextLine().fromFirstOccurrenceOf("//", false, false).upToLastOccurrenceOf("\n", false, false);
                    String cppLogonName = cppStream.readNextLine().fromFirstOccurrenceOf("by ", false, false).upToLastOccurrenceOf("\n", false, false);
                    String cppFullUserName = cppStream.readNextLine().fromFirstOccurrenceOf("by ", false, false).upToLastOccurrenceOf("\n", false, false);
                    
                    std::cout << logonName2 << " - " << cppLogonName << "\n";
                    std::cout << getFullUserName2 << " - " << cppFullUserName << "\n";
                    
                    String reverseCheckSum = AserveUnitTest::getCheckSum(cppLogonName);
//                    jassert(reverseCheckSum == checkSum);
                    if (reverseCheckSum != checkSum) {
                        std::cout << "Checksum error!! " << fCpp.getFullPathName() << "\n";
                    }
                    if (logonName2 != cppLogonName || getFullUserName2 != cppFullUserName) {
                        std::cout << "File properties error!! " << fCpp.getFullPathName() << "\n";
                    }
                    
//                    jassert(logonName2 == cppLogonName);
//                    jassert(getFullUserName2 == cppFullUserName);
                    
//                    jassert(logonName2 == logonName);
//                    jassert(getFullUserName2 == getFullUserName);
//                    jassert(CPUName2 == CPUName);
                }
                
            }
            
            
        }
        
    }
};

#endif /* TheftChecker_hpp */
