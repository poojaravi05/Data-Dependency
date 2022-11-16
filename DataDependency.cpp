#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

/*
 * Given an instruction string, extract the input variables
 * and push it to a set (to avoid any repetations of variables)
 */
set<char> getInputVariables(string instruction)
{
    set<char> ret;

    for (int i = 1; i < instruction.size(); i++) {
        /*
         * Checks if the character is a variable and not an operator.
         * Here the alphabets represent the variables.
         */
        if (isalpha(instruction[i])) {
            ret.insert(instruction[i]); 
        }
    }

    return ret;
}

/*
 * Debug helper function to check if
 * the input variables are parsed fine.
 */ 
void printInputVariables(set<char> input)
{
    for (auto x : input) {
        cout << x << "\t";
    }
    cout << endl;
}

bool isFlowDependent(char simpleInstructionOutputVariable, set<char> blockInstructionInputVariables)
{
    /*
     * checks if output(simpleinstruction) intersection of input(blockinstruction) = null
     */
    if (blockInstructionInputVariables.find(simpleInstructionOutputVariable) != \
        blockInstructionInputVariables.end()) {
        return true; 
    }

    return false;
}

bool isAntiDependent(char blockInstructionOutputVariable, set<char> simpleInstructionInputVariables)
{
    /*
     * checks if input(simpleinstruction) intersection of output(blockinstruction)= null
     */
    if (simpleInstructionInputVariables.find(blockInstructionOutputVariable) != \
        simpleInstructionInputVariables.end()) {
        return true; 
    }

    return false;
}

bool isOutputDependent(char simpleInstructionOutputVariable, char blockInstructionOutputVariable)
{
    /*
     * checks if output(simpleinstruction) intersection of input(blockinstruction)= null
     */
    return (simpleInstructionOutputVariable == blockInstructionOutputVariable);
}

/*
 * Implement AlgoVerify:
 * -----------------------
 * Given a set of BlockInstructions, we compare each instruction
 * with the rest of the instructions individually and verify if
 * it satisfies all the dependencies and conditions for instructions
 * to be executed in parallel.
 * If yes, we add the parallel set of instructions to result.
 * else, we move on to the next iteration.
 */
vector<vector<string>> AlgoVeirfy(vector<string> BlockInstructions)
{
    vector<vector<string>> result;
    int blockInstructionSize = BlockInstructions.size();

    for (int i = 0; i < blockInstructionSize - 1; i++) {
        char simpleinstructionOutput = BlockInstructions[i][0]; // get the output variable of 1st comparison instruction
        set<char> simpleInstructionInputVariables = getInputVariables(BlockInstructions[i]); // get input variables

        for (int j = i + 1; j < blockInstructionSize; j++) {
            char blockInstructionOutputVariable = BlockInstructions[j][0]; // get the output variable of 2nd comparison instruction
            set<char> blockInstructionInputVariables = getInputVariables(BlockInstructions[j]); // get the input variables
            
            if (!isFlowDependent(simpleinstructionOutput, blockInstructionInputVariables) && \
                !isAntiDependent(blockInstructionOutputVariable, simpleInstructionInputVariables) && \
                !isOutputDependent(simpleinstructionOutput, blockInstructionOutputVariable)) {
                /*
                 * if all the dependencies satisfy the condition for a pair of instructions, 
                 * they are parallel instruction set
                 */
                vector<string> parallelInstrucitonsSet {BlockInstructions[i], BlockInstructions[j]};
                result.push_back(parallelInstrucitonsSet); // parallel executed instructions are pushed to result
            }
        }
    }

    return result;
}

/*
 * Implement AlgoCalculate:
 * -------------------------
 * Given a SimpleInstruction and a set of BlockInstructions,
 * we compare each BlockInstruction against the given SimpleInstruction
 * and verify if it satisfies all the dependencies and conditions 
 * for instructions to be executed in parallel.
 * If yes, we add the parallel set of instructions to result.
 * else, we move on to the next iteration.
 */
vector<vector<string>> AlgoCalculate(string SimpleInstructions, vector<string> BlockInstructions)
{
    vector<vector<string>> result;
    int blockInstructionSize = BlockInstructions.size();

    char simpleinstructionOutput = SimpleInstructions[0]; //getting the output variable of the simple instruction
    set<char> simpleInstructionInputVariables = getInputVariables(SimpleInstructions); // getting the input variable
    
    for (int i = 0; i < blockInstructionSize; i++) {
        char blockInstructionOutputVariable = BlockInstructions[i][0]; // getting the output variable of block instruction
        set<char> blockInstructionInputVariables = getInputVariables(BlockInstructions[i]); // getting the input variable
        
        if (!isFlowDependent(simpleinstructionOutput, blockInstructionInputVariables) && \
            !isAntiDependent(blockInstructionOutputVariable, simpleInstructionInputVariables) && \
            !isOutputDependent(simpleinstructionOutput, blockInstructionOutputVariable)) {
            /*
             * if all the dependencies satisfy the condition for a pair of instructions, 
             * they are parallel instruction set
             */
            vector<string> parallelInstrucitonsSet {SimpleInstructions, BlockInstructions[i]};
            result.push_back(parallelInstrucitonsSet); // parallel executed instructions are pushed to result
        }
    }

    return result;
}

/*
 * Helper function to print the result.
 * It prints the set of instructions that can be run in parallel.
 */
void printParallelSetInstructions(vector<vector<string>> input)
{
    if (input.size() == 0) {
        cout << "The pairs of instructions that can be executed in parallel are: NONE" << endl;
        return;
    }

    cout << "The pairs of instructions that can be executed in parallel are: " << input.size() << endl;
    cout << "Printing the set of Parallel Instructions: " <<endl;
    
    for (auto x : input) {
        for (auto y : x) {
            cout << y << "\t\t"; 
        }
        cout << endl;
    }
    cout <<endl;
}

int main()
{
    vector<string> blockOfInstructions{"b = b * c", "d = c - a", "a = a + b * c"}; // input: set of block instructions
    string simpleInstruction("d = b + ( c - d / e)"); // input: simple instruction 
    vector<vector<string>> parallelInstructionsSets;

    cout << endl <<"Running algo verify..."<<endl; 
    parallelInstructionsSets = AlgoVeirfy(blockOfInstructions);
    printParallelSetInstructions(parallelInstructionsSets);
    
    
    cout << endl << endl << "Running algo calculate..."<<endl;
    parallelInstructionsSets = AlgoCalculate(simpleInstruction, blockOfInstructions);
    printParallelSetInstructions(parallelInstructionsSets);

    return 0;
}
