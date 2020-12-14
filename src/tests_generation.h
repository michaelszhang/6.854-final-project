#include "heap.h"
#include "item.h"

#include <map>

#define EPSILON 1e-9
#define NUM_OPERATIONS 3
enum OPERATION {INSERT=0, DECREASE_KEY=1, DELETE_K=2};

class DecreaseKeySampler {
public:
	void add(INode* n);
	void remove(INode* n);
	INode* sampleUniformTime();

private:
	std::map<INode*, double> time_inserted;
	std::map<double, INode*> distribution;
	int time = 0;
};

double randomUniform();
std::vector<int> value_sequence(int n, double alpha);
int next_state(int *operations_left, std::vector<double> &pdf);
std::vector<int> operation_sequence(std::vector<std::vector<double> > &transitions, int n0, int n1, int k);
