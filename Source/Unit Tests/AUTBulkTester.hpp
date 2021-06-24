//
//  AUTBulkTester.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 06/08/2019.
//

#ifndef AUTBulkTester_hpp
#define AUTBulkTester_hpp

#include "AserveUnitTest.hpp"


#include "../JuceLibraryCode/JuceHeader.h"
#include "AserveComs.h"
#include "AUTMtof.hpp"
#include "AUTTestTone.hpp"
#include "AUTDrumSampler.hpp"
#include "ComponentList.h"
#include "UnitTestUI.hpp"

class AUTBulkTester : public Component, public ActionListener, public Thread {
    
public:
    
    struct Student {
        Student ()
        {
            for (int i = 0; i < 10;i++)
            {
                scores[i] = 0;
            }
            hasHiddenFile = false;
        }
        File fRoot;
        String name;
        String email;
        int testsPassed;
        int testsFailed;
        int testsNotAttempted;
        int scores[10];
        String number;
        bool hasHiddenFile;
    };
    
    AUTBulkTester (AserveComs & coms);
    ~AUTBulkTester ();
    
    void paint (Graphics &g) override;
    void resized () override;
    
    void actionListenerCallback (const String& message) override;
    
    
    TestSelector * getSelectorForName (String name);
    void resetSelectors (int idSelect);
    
    
    void runBulkTestStaff (File fPath);
    void runFolderOfTestStudent (File fPath);
    
    void run() override;
    
    static String aserveBulkTesterPath;
    static String staffMarkingSolutionsFolder;
    static String sourceFolderPath;
    static String executablePath;
    static String xcodeProjPath;
    static int numberOfTestsToRun;
    
    void printAllResultsStaff ();
    void restoreStudentIAPFiles ();
private:
    ScopedPointer<AserveUnitTest> unitTest;
    String selectedTest;
    
    AserveComs & coms;
    File fPath;
    std::vector<Student> students;
    int studentIndex;
    int testIndex;
    
    void nextTest ();
    bool revertOriginalSourceFiles;
};


#endif /* AUTBulkTester_hpp */
