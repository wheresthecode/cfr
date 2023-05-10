#include "solvertest2.h"
#include <math.h>
using namespace std;

enum Actions
{
	Action_Rock = 0,
	Action_Paper,
	Action_Scissors,
	Action_Count,
};

const char* kActionNames[] = {
	"Rock",
	"Paper",
	"Scissors"
};

int resultMatrix[Action_Count][Action_Count] = {
	{0, -1, 1},
	{1, 0, -1},
	{-1, 1, 0}
};

class RPSTrainer
{
public:
	double m_RegretSum[Action_Count] = {};
	double m_Strategy[Action_Count] = {};
	double m_StrategySum[Action_Count] = {};
	double m_OppStrategy[Action_Count];

	RPSTrainer() :
		m_OppStrategy{ 0.4, 0.3, 0.3 }
	{
	}

	Actions ChooseAction(double* strategy)
	{
		double random_number = static_cast<double>(rand()) / RAND_MAX;
		for (int i = 0; i < Action_Count; i++)
		{
			if (random_number < strategy[i])
				return (Actions)i;
			random_number -= strategy[i];
		}
		return (Actions)(Action_Count - 1);
	}

	double Sum(double* v, int c) {
		double sum = 0.0;
		for (int i = 0; i < c; i++)
			sum += v[i];
		return sum;
	}

	void Normalize(double* inV, double* outV, int c)
	{
		double sum = Sum(inV, c);
		for (int i = 0; i < c; i++)
			outV[i] = sum != 0.0 ? (inV[i] / sum) : (1.0 / c);
	}

	void UpdateStrategy()
	{
		double sum = 0.0;
		for (int i = 0; i < Action_Count; i++)
		{
			m_Strategy[i] = std::max(m_RegretSum[i], 0.0);
			sum += m_Strategy[i];
		}
		for (int i = 0; i < Action_Count; i++)
		{
			m_Strategy[i] = sum > 0.0 ? (m_Strategy[i] / sum) : (1.0 / Action_Count);
			m_StrategySum[i] += m_Strategy[i];
		}
	}

	void PrintStrategy(std::string title, double* strategy)
	{
		cout << title << endl;
		for (int i = 0; i < Action_Count; i++)
			cout << kActionNames[i] << ": " << strategy[i] << endl;
	}

	void Train()
	{

		for (int iter = 0; iter < 10000; iter++)
		{
			Actions myAction = ChooseAction(m_Strategy);
			Actions oppAction = ChooseAction(m_OppStrategy);

			// Apply utility to regret
			for (int i = 0; i < Action_Count; i++)
			{
				m_RegretSum[i] += resultMatrix[i][oppAction] - resultMatrix[myAction][oppAction];
			}

			UpdateStrategy();
		}

		double avgStrategy[Action_Count];
		Normalize(m_StrategySum, avgStrategy, Action_Count);

		PrintStrategy("Opp strategy is:", m_OppStrategy);
		PrintStrategy("Optimal Strategy against opp: ", avgStrategy);
	}
	
};

int main()
{
	RPSTrainer trainer;
	trainer.Train();

	return 0;
}
