/**
 * The Matrixinator v0.7 (beta)
 * a simple utility i made for my assistant Joshu for his laboratory endeavours.
 * its purpose lies in fetching a dendrogram containing samples and the similarity
 * between them, along with a spreadsheet file to identify foreign samples from US samples.
 *
 * using the dendrogram provided in a .xml file, it finds the similarity between any two
 * samples provided.
 * 
 * after that, it's capable of inferring the following values:
 * BS22, BS15, BS3, BS8, BS27, BS84, BS18, BS278. 
 * these calculations are made based on the found similarities.
 *
 * it then outputs the inferred values (if applicable) into a .csv spreadsheet. it can either
 * overwrite the input spreadsheet file or create a new one, depending on the user's choice.
 * both the input and output spreadsheets have about the same format.
 *
 * -Leo, 12-Feb-2020
 */

#include "utils.hpp"
#include "mtx.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Fmtx mtx;

    //prelude
    mtx.prelude();

    //input
    mtx.nline();
    mtx.UI();
    mtx.outputOptions();

    //processing
    mtx.nline();
    mtx.readSelves();
    mtx.postInit();
    mtx.sweep();

    //output
    mtx.nline();
    mtx.output();

    (mtx.pottyMouth) ?
        std::cout << "see ya, bitch." :
        std::cout << "Terminating program execution.";

    sysexit(0);
}