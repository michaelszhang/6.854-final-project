#include "heap.h"
#include "item.h"

#include <map>
#include <set>

#define EPSILON 1e-9
#define NUM_OPERATIONS 3
enum OPERATION {INSERT=0, DECREASE_KEY=1, DELETE_K=2};

class DecreaseKeySampler {
public:
	DecreaseKeySampler(int num_total);

	void add(INode* n);
	void remove(INode* n);
	INode* sampleUniformTime();
	int next_unique_key(int key);

	unsigned size();

private:
	std::map<INode*, double> time_inserted;
	std::map<double, INode*> distribution;
	std::set<int> available_keys;
	int time = 0;
	int num_items = 0;
};

double randomUniform();
std::vector<int> value_sequence(int n, double alpha);
int next_state(int *operations_left, std::vector<double> &pdf);
std::vector<int> operation_sequence(std::vector<std::vector<double> > &transitions, int n, int k);
