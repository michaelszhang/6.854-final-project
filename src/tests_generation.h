#include "heap.h"
#include "item.h"

#include <map>

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
