/*
 * classes header file for the almighty matrixinator
 * --------------------------------------------------------------------------------------------------
 * Fmtx: class 'functional matrix'
 *
 * this class was made to store critical data relating to the matrixinator's operations, including
 * vectors of the spreadsheet and dendrogram, statistics such as number of samples/nodes, folder paths,
 * along with helper methods (private) and the main program sequence methods (public).
 *
 * there is a similarity matrix struct down below, if it's ever needed to build one again.
 *
 * --------------------------------------------------------------------------------------------------
 * SSheet: class 'spreadsheet'
 *
 * this class is meant simply for storage of the entire metadata .csv file, with methods to copy
 * the file to memory and write the memory's contents into a file. it may be used to create an entirely
 * new file or overwrite a currently existing spreadsheet (truncate).
 * 
 * --------------------------------------------------------------------------------------------------
 * Tree: class 'tree'
 *
 * this class reads and stores a dendrogram tree read from a .xml file and its nodes' attributes.
 * the similarity value between any two samples is the similarity value of the closest compound node that
 * contains both samples.
 *
 * -Leo, 12-Feb-2020
*/

#ifndef MTX_HPP_
#define MTX_HPP_

#include <string>
#include <vector>
#include <set>
#include <filesystem>

class Fmtx;
class Tree;
class SSheet;

// similarity matrix (if needed)
/*
typedef struct {
private:
    std::vector<double> similarities;
    int indexes;
} Matrix;
*/

// functional + statistical matrixinator
class Fmtx {
    std::vector<SSheet> SS; //schutzstaffel (^:
    std::vector<Tree> acacia;

    std::vector<int> USAlist;
    int numNodes;
    int numSamples;
    std::string inputFolder, inputSS, inputTree, outputSS; //paths
    bool iniPowered = false;

    //helpers
    void readFiles(bool);
    void promptPath();
    std::string fileName(std::string, bool);
    std::string path2string(std::filesystem::directory_entry);
    bool isLoaded(std::string);
    double bullSim(int, int);
    void bulldozer(int);
    bool isUS(int);

public:
    bool pottyMouth = false; //for LAUGHERS
    void nline();

    //main sequence
    void UI();
    void outputOptions();
    void readSelves();
    void postInit();
    void sweep();
    void output();
};

// metadata spreadsheet
class SSheet {
    double octagon[8];
    int node; //node-sample association

    std::string Key, Location, CollectionDate, Company, FSGID, Farm, Age_days, SampleOrigin,
    SampleType, VMP, ibeA, traT, iutA, ompT, sitA, irp2, cvaC, tsh, iucC, iss;
    //BS22, BS15, BS3, BS8, BS27, BS84, BS18, BS278 = octagon values (for title purposes)

    void writeSelf(std::string, bool, bool);
    void readSelf(std::string, int);

    friend class Fmtx;
};

// dendrogram similarity tree
class Tree {
    int ID, parentID;
    double similarity;
    std::set<int> list;
    bool isSample;

    void readSelf(std::string, int);

    friend class Fmtx;
};

#endif /* MTX_HPP_ */
