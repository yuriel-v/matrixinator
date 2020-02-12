/*
 * method implementation file: the matrixinator
 * for more details, check the header file.
 * 
 * -Leo, 12-Feb-2020
 * 
*/

#include "utils.hpp"
#include "mtx.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <cstddef>
#include <cmath>
//#include <string>
//#include <set>
//#include <vector>
//#include <filesystem>

// ========== I/O methods from Tree and SSheet ==========
// all three of these will read or write a single line between memory and file, both ways.
// iteration is the way of the future here.

void Tree::readSelf(std::string filePath, int line)
{
    std::fstream xml (filePath, std::fstream::in);
    for (int i = 0; i < line; ++i)
        xml.ignore(2048, '/');
    std::string buffer;

    for (int i = 0; i < 3; ++i) {
        xml.ignore(1000, '\"');
        std::getline(xml, buffer, '\"');
        switch (i) {
        case 0:
            std::stringstream(buffer)>> ID; break;
        case 1:
            std::stringstream(buffer)>> parentID; break;
        case 2:
            std::stringstream(buffer)>> similarity; break;
        }

        char c = xml.peek();
        isSample = (c == '/')? false : true;
    }
    xml.close();
}

// SSheet

void SSheet::readSelf(std::string filePath, int line) {
    std::fstream sprSheet (filePath, std::fstream::in);

    for (int i = 0; i <= line; ++i)
        sprSheet.ignore(2048, '\n');
    sprSheet.precision(14);

    std::getline(sprSheet, Key, ',');
    std::getline(sprSheet, Location, ',');
    std::getline(sprSheet, CollectionDate, ',');
    std::getline(sprSheet, Company, ',');
    std::getline(sprSheet, FSGID, ',');
    std::getline(sprSheet, Farm, ',');
    std::getline(sprSheet, Age_days, ',');
    std::getline(sprSheet, SampleOrigin, ',');
    std::getline(sprSheet, SampleType, ',');
    std::getline(sprSheet, VMP, ',');
    std::getline(sprSheet, ibeA, ',');
    std::getline(sprSheet, traT, ',');
    std::getline(sprSheet, iutA, ',');
    std::getline(sprSheet, ompT, ',');
    std::getline(sprSheet, sitA, ',');
    std::getline(sprSheet, irp2, ',');
    std::getline(sprSheet, cvaC, ',');
    std::getline(sprSheet, tsh, ',');
    std::getline(sprSheet, iucC, ',');
    std::getline(sprSheet, iss, ',');

    // get octagon values from US samples only
    if (Location == "US" || Location == "USA") {
        std::string buffer;
        for (int j = 0; j < 8; ++j) {
            std::getline(sprSheet, buffer, ',');
            std::stringstream(buffer) >> octagon[j];
        }
    }
    sprSheet.close();
}

void SSheet::writeSelf(std::string filePath, bool truncate, bool title) {
    std::fstream output;
    (truncate)?
        output.open(filePath, std::fstream::out | std::fstream::trunc):
        output.open(filePath, std::fstream::out | std::fstream::app);
    output.precision(14);

    if (!output.is_open()) {
        printf("\nError writing to file!\n");
        return;
    }

    if (title) output<<"Key,Location,CollectionDate,Company,FSGID,Farm,Age_days,SampleOrigin,SampleType,VMP,ibeA,traT,iutA,ompT,sitA,irp2,cvaC,tsh,iucC,iss"
                  <<",BS22,BS15,BS3,BS8,BS27,BS84,BS18,BS278,\n";
    else {
        output << Key <<","
               << Location <<","
               << CollectionDate <<","
               << Company <<","
               << FSGID <<","
               << Farm <<","
               << Age_days <<","
               << SampleOrigin <<","
               << SampleType <<","
               << VMP <<","
               << ibeA <<","
               << traT <<","
               << iutA <<","
               << ompT <<","
               << sitA <<","
               << irp2 <<","
               << cvaC <<","
               << tsh <<","
               << iucC <<","
               << iss;


        for (int j = 0; j < 8; ++j) {
            (isnan(octagon[j])) ?
                output << "," :
                output << "," << octagon[j];
        }

        output << ",\n";
    }
    output.close();
}

// ========== Statistical and Functional Matrixinator's methods ==========

// public:

// do i really need to explain this?
void Fmtx::nline() { std::cout<<"\n==================================================\n"; }

// first run function, interacts with user to determine input files and output options
void Fmtx::UI()
{
    using namespace std;

    fstream ini("mtx.ini", fstream::in);
    if (ini.is_open()) {
        (pottyMouth) ?
            cout << "hey cunt, there's an ini file here. should we load folders from it? (Y/N)\n" :
            cout << "Initialization file detected. Would you like to load folders from it? (Y/N)\n";
        if (YN()) {
            (pottyMouth) ?
                cout << "sure, let's see then... " :
                cout << "Understood. Loading folders... ";

            ini.ignore(500, '=');
            getline(ini, inputFolder);

            cout << "done." << endl;

            iniPowered = true;
            readFiles(false);
        }
        else {
            (pottyMouth) ?
                cout << "you're a nigger, giving me more work to do. let's do it the hard way then.\n" :
                cout << "Understood. Commencing manual load.\n";
            readFiles(true);
        }
        ini.close();
    }
    else readFiles(true);
}

// overwrite?
void Fmtx::outputOptions()
{
    using namespace std;
    if (!iniPowered) {
        (pottyMouth) ?
            cout << "you wanna save some time and put this folder in an ini file? (Y/N)" << endl :
            cout << "Would you like to save the current folder path to an initialization file? (Y/N)" << endl;

        if (YN()) {
            (pottyMouth) ?
                cout << "wise choice. saved on the same folder as the program, named \"mtx.ini\".\n\n" :
                cout << "Understood. A file named \"mtx.ini\" has been created on the same folder as this executable.\n\n";
            fstream ini("mtx.ini", fstream::out | fstream::trunc);
            ini << "[Matrixinator initialization file]\n"
                << "FilePath=" << inputFolder << endl;
            ini.close();
        }
        else {
            (pottyMouth) ?
                cout << "well, fuck then. let's resume.\n\n" :
                cout << "Understood. No changes were made.\n\n";
        }
    }

    (pottyMouth) ?
        cout << "wanna overwrite the original spreadsheet? (Y/N)\n" :
        cout << "Would you like to overwrite the original metadata spreadsheet? (Y/N)\n";

    if (YN()) {
        (pottyMouth) ?
            cout << "fine, let's overwrite this shit, nigga.\n"
                 << "press enter to get started.\n" :
            cout << "Understood. The current spreadsheet file will be overwritten with processed content.\n"
                 << "Press enter to begin execution.\n";
        outputSS = inputFolder + inputSS; cin.get();
    }

    else {
        (pottyMouth) ?
            cout << "ok but nigga, i'ma make a file named \"output.csv\" on the same folder as the original.\n"
                 << "if i see another file there with the same name i'ma SHANK A BITCH and toss it out the window.\n"
                 << "press enter to get started.\n" :
            cout << "Understood. The output file will be named \"output.csv\", in the same folder as the original spreadsheet.\n"
                 << "WARNING: If there is already a file with the same name in that folder, it will be OVERWRITTEN!\n"
                 << "Press enter to begin execution.\n";
        outputSS = inputFolder + "output.csv"; cin.get();
    }
    return;
}

// resizing tree/ssheet vectors so they can fit the right amount of data
void Fmtx::readSelves()
{
    // at this point, paths have been tested and proven so we can do this
    using namespace std;
    inputSS = inputFolder + inputSS;
    inputTree = inputFolder + inputTree;

    // identify US samples + sample amount
    fstream file(inputSS, fstream::in);
    file.ignore(INT_MAX, '\n');
    string in; int cnt = 0;

    while (!file.eof()) {
        in.clear();
        file.ignore(100, ','); // key
        getline(file, in, ',');

        if (in.length() < 2) break;

        if (in == "US" || in == "USA")
            USAlist.push_back(cnt);

        file.ignore(INT_MAX, '\n');
        ++cnt;
        ++numSamples;
    }
    file.close();

    // count number of nodes in .xml
    file.open(inputTree, fstream::in | fstream::ate);
    cnt = 0;

    while (cnt < 3) {
        file.seekg(-1, ios_base::cur);
        if (file.peek() == '=') ++cnt;
    }
    file.ignore(5, '\"');
    getline(file, in, '\"');
    stringstream(in) >> numNodes;
    file.close();

    // resize vectors - better ++ and -- than have intellisense on my ass about it. perfectionism.
    ++numNodes;
    SS.resize(numSamples);
    acacia.resize(numNodes);
    --numNodes;

    // read dendrogram
    (pottyMouth) ?
        cout << "lemme read this shitty dendrogram... " :
        cout << "Loading dendrogram to memory... ";
    acacia[0].ID = 0;
    acacia[0].parentID = 0;
    acacia[0].similarity = 0;
    acacia[0].isSample = false;
    for (int i = 1; i <= numNodes; ++i)
        acacia[0].list.insert(i);

    for (int i = 1; i <= numNodes; ++i)
        acacia[i].readSelf(inputTree, i - 1);
    cout << "done.\n";

    // read metadata
    (pottyMouth) ?
        cout << "ok, now for the spreadsheet... " :
        cout << "Loading metadata to memory... ";
    for (int i = 0; i < numSamples; ++i)
        SS[i].readSelf(inputSS, i);
    cout << "done.\n";

}

void Fmtx::postInit()
{
    using namespace std;
    (pottyMouth) ?
        cout << "--\nok, let's begin post-init, nigga.\n" :
        cout << "--\nCommencing post-initialization.\n";

    // branching
    (pottyMouth) ?
        cout << "- pissing on tree... " :
        cout << "- Branching tree... ";
    for (int i = 1; i <= numNodes; ++i)
        if (acacia[i].isSample)
            bulldozer(i);
    (pottyMouth) ?
        cout << "feels gud.\n" :
        cout << "done.\n";

    // sample-node association
    (pottyMouth) ?
        cout << "- connecting the dots... " :
        cout << "- Performing sample-node association... ";
    int sCount = 0;
    for (int i = 1; i <= numNodes; ++i) {
        if (acacia[i].isSample) {
            SS[sCount].node = i;
            ++sCount;
        }
    }
    (pottyMouth) ?
        cout << "i'm smurt!\n" :
        cout << "done.\n";
}

void Fmtx::sweep()
{
    using namespace std;
    (pottyMouth) ?
        cout << "--\nok, time for the hard shit now. let's get into the octagon.\n" :
        cout << "--\nSweeping samples.\n";
    cout << "-> ";

    for (int foreign = 0; foreign < numSamples; ++foreign) {
        
        if (isUS(foreign)) {
            cout << "\r-> " << foreign + 1 << " sample(s) processed.";
            continue;
        }

        int caseCount = 0; vector<int> matches;
        for (vector<int>::const_iterator it = USAlist.begin(); it != USAlist.end(); ++it)
            if (bullSim(SS[foreign].node, SS[*it].node) >= 80) {
                ++caseCount;
                matches.push_back(*it);
            }

        switch (caseCount) {
            // case 0, do nothing 
        case 1:
            for (int i = 0; i < 8; ++i)
                SS[foreign].octagon[i] = SS[matches[0]].octagon[i];
            break;
        default:

            // multiply value by the similarity of that particular usa sample to the foreign sample
            vector<vector<double>> tempOct(matches.size()); // msvc is a BITCH
            for (int i = 0; i < matches.size(); i++)
                tempOct[i].resize(8);

            int i = 0;
            for (vector<int>::iterator it = matches.begin(); it != matches.end(); ++it) {
                double curSim = bullSim(SS[foreign].node, SS[*it].node);
                for (int j = 0; j < 8; ++j)   // octagon value #j
                    tempOct[i][j] = SS[*it].octagon[j] * curSim;
                ++i;
            }

            // add up all the similarities for every usa sample matched
            double tempSim = 0;
            for (vector<int>::iterator it = matches.begin(); it != matches.end(); ++it)
                tempSim += bullSim(SS[foreign].node, SS[*it].node);

            // add up all of the octagon values for every usa sample
            double foreignOctagon[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            for (int i = 0; i < (int)matches.size(); ++i)
                for (int j = 0; j < 8; ++j)
                    foreignOctagon[j] += tempOct[i][j];

            // divide by the total similarity + print to octagon for foreign sample
            for (int i = 0; i < 8; ++i) {
                foreignOctagon[i] /= tempSim;
                SS[foreign].octagon[i] = foreignOctagon[i];
            }
        }

        cout << "\r-> " << foreign + 1 << " sample(s) processed.";
    }
    cout << '\n';

}

void Fmtx::output()
{
    (pottyMouth) ?
        std::cout << "time to wrap this shit up, at last... " :
        std::cout << "Writing output spreadsheet... ";
    SS[0].writeSelf(outputSS, true, true); // title first, content later
    for (int i = 0; i < numSamples; ++i)
        SS[i].writeSelf(outputSS, false, false);
    std::cout << "done.\n=====\n";
}

// private:

// check if a sample is foreign
bool Fmtx::isUS(int index) {
    for (std::vector<int>::const_iterator it = USAlist.begin(); it != USAlist.end(); ++it)
        if (*it == index) return true;
    return false;
}

// carry all the node IDs from sample to root, adding them to the nodes' lists along the way
void Fmtx::bulldozer(int node)
{
    if (!acacia[node].isSample) return;

    std::vector<int> changeList;
    int parent = acacia[node].parentID;

    while (acacia[parent].ID >= 1) {
        changeList.clear();
        changeList.push_back(acacia[node].ID);
        node = parent;
        parent = acacia[parent].parentID;

        // add the change list to the parent's list
        for (std::vector<int>::const_iterator it = changeList.begin(); it != changeList.end(); ++it)
            (acacia[node].list).insert(*it);
    }
    // we're at node 1 at this point, parent ID 0
    for (std::vector<int>::const_iterator it = changeList.begin(); it != changeList.end(); ++it)
        (acacia[node].list).insert(*it);
}

// compare node's similarity to compID
double Fmtx::bullSim(int node, int compID) {
    if (!acacia[node].isSample) return 0;

    int parent = acacia[node].parentID;

    while (acacia[parent].ID >= 1) {
        // the almighty time saver
        if (acacia[node].similarity < 80)
            return 0;

        for (std::set<int>::const_iterator it = (acacia[parent].list).begin(); it != (acacia[parent].list).end(); ++it)
            if (*it == compID)
                return acacia[parent].similarity;

        node = parent;
        parent = acacia[parent].parentID;
    }
    // deprecated fail-safe
    return acacia[node].similarity;
}

// simple prompt for asking a path
void Fmtx::promptPath()
{
    (pottyMouth) ?
        std::cout << "--\ngimme a folder to read from, faggot.\n>> " :
        std::cout << "--\nPlease input a folder to read from.\n>> ";
    std::getline(std::cin, inputFolder);
}

// return file names or extensions
std::string Fmtx::fileName(std::string str, bool extension)
{
    std::size_t found;
    found = (extension) ?
        str.find_last_of('.') :
        str.find_last_of("/\\");

    str = str.substr(found + 1);

    if (extension) std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// convert a path object to string
std::string Fmtx::path2string(std::filesystem::directory_entry p)
{
    std::filesystem::path buffer = p.path();
    return buffer.string();
}

// check if a file exists
bool Fmtx::isLoaded(std::string path) {
    bool ret;
    std::fstream file(path, std::fstream::in);
    ret = file.is_open();
    file.close();
    return ret;
}

// after fetching path off first run, cycle through that folder and let the user pick which files to load to memory
void Fmtx::readFiles(bool manual)
{
    namespace fs = std::filesystem;
    using namespace std;

    string file;
    bool loaded = false, firstTry = true, xmlFound = false, csvFound = false;
    vector<string> csvFiles, xmlFiles;

    while (!loaded) {
        if (!(firstTry && !manual))
            promptPath();
        csvFiles.clear(); xmlFiles.clear();
        try {
            // populate xml/csv lists
            for (auto& p : fs::directory_iterator(inputFolder)) {
                file = path2string(p);
                if (fileName(file, true) == "XML") {
                    xmlFiles.push_back(fileName(file, false));
                    xmlFound = true;
                }
                else if (fileName(file, true) == "CSV") {
                    csvFiles.push_back(fileName(file, false));
                    csvFound = true;
                }
            }
        }
        catch (...) {
            (pottyMouth) ?
                cout << "you fucking nigger, i can't access that folder, if it even exists!\n" :
                cout << "Exception thrown: the folder provided doesn't exist or is otherwise inaccessible!\n";
            firstTry = false; continue;
        }

        if (!(xmlFound && csvFound)) {
            if (!xmlFound && csvFound) {
                (pottyMouth) ?
                    cout << "this shithole has no .xml files, cunt! try again.\n" :
                    cout << "The specified folder does not contain any .xml files! Try again.\n";
            }
            else if (xmlFound && !csvFound) {
                (pottyMouth) ?
                    cout << "this shithole has no .csv files, cunt! try again.\n" :
                    cout << "The specified folder does not contain any .csv files! Try again.\n";
            }
            else {
                (pottyMouth) ?
                    cout << "this shithole ain't got nothing relevant here, you moron! try a folder with .xml and .csv files.\n" :
                    cout << "The specified folder does not contain any .csv or .xml files! Try again.\n";
            }

            firstTry = false; continue;
        }
        else {
            int i = 0;
            (pottyMouth) ?
                cout << "\nhere's what i found.\n"
                     << "format: [index] filename.extension\n" :
                cout << "\nDisplaying a list of all available files found.\n"
                     << "Format: [index] filename.extension\n";

            cout << "\n.csv files:\n";
            for (vector<string>::const_iterator it = (csvFiles).begin(); it != csvFiles.end(); ++it) {
                cout << "- [" << i << "] " << *it << '\n';
                ++i;
            }

            i = 0;
            cout << "\n.xml files:\n";
            for (vector<string>::const_iterator it = (xmlFiles).begin(); it != xmlFiles.end(); ++it) {
                cout << "- [" << i << "] " << *it << '\n';
                ++i;
            }

            (pottyMouth) ?
                cout << "\nthis folder got what you want? (Y/N)\n" :
                cout << "\nDoes this folder contain the files you're looking for? (Y/N)\n";
            if (YN()) {
                bool xmlLoaded = false, csvLoaded = false;

                // quick check for no fuckups on file structure
                if (inputFolder.at(inputFolder.length() - 1) != '/' || inputFolder.at(inputFolder.length() - 1) != '\\')
                    inputFolder += '/';

                while (!(xmlLoaded && csvLoaded)) {
                    unsigned xmlIndex = 0, csvIndex = 0;
                    
                    inputTree.clear(); inputSS.clear();

                    (pottyMouth) ?
                        cout << "where the dendrogram (.xml) at?\n>> " :
                        cout << "Please type the index of the dendrogram tree (.xml) file.\n>> ";
                    cin >> xmlIndex; inflush();
                    (pottyMouth) ?
                        cout << "where the metadata (.csv) at?\n>> " :
                        cout << "Please type the index of the metadata spreadsheet (.csv) file.\n>> ";
                    cin >> csvIndex; inflush();
                    (pottyMouth) ?
                        cout << "ok let's see then... " :
                        cout << "Loading files to memory... ";

                    if (xmlIndex >= xmlFiles.size() || csvIndex >= csvFiles.size()) {
                        (pottyMouth) ?
                            cout << "nice try, asshole.\n"
                                 << "you typed an index out of range. try again, cunt.\n\n" :
                            cout << "error!\n"
                                 << "You typed an index out of range. Please try again.\n\n";
                        continue;
                    }
                    else {
                        inputTree = xmlFiles.at(xmlIndex);
                        inputSS = csvFiles.at(csvIndex);
                    }

                    xmlLoaded = isLoaded(inputFolder + inputTree);
                    csvLoaded = isLoaded(inputFolder + inputSS);

                    if (xmlLoaded && csvLoaded) { cout << "done.\n\n"; loaded = true; }
                    else {
                        cout << "failed.\n"
                             << "-> Dendrogram tree (.xml): ";
                        (xmlLoaded) ?
                            cout << "loaded.\n" :
                            cout << "failed.\n";
                        cout << "-> Metadata spreadsheet (.csv): ";
                        (csvLoaded) ?
                            cout << "loaded.\n" :
                            cout << "failed.\n";

                        (pottyMouth) ?
                            cout << "would your stupidness like to change folders? (Y/N)\n" :
                            cout << "Would you like to change the folder? (Y/N)\n";
                        if (YN()) break;
                    }
                }
            }
            else { firstTry = false; continue; }
        }
    }
}