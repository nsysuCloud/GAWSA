#ifndef __GA__
#define __GA__

#include <vector>
#include <string>
using namespace std;

typedef vector<int> i1d;
typedef vector<i1d> i2d;
typedef vector<i2d> i3d;
typedef vector<float> f1d;
typedef vector<f1d> f2d;
typedef vector<f2d> f3d;

typedef vector<string> s1d;
typedef vector<double> d1d;
typedef vector<d1d> d2d;
typedef vector<d2d> d3d;

typedef struct Task
{
    int i, j;
    double start, end;
    int num_resource;
} Task;

typedef struct Resource
{
    vector<Task> WorkSet;
    double execute_time;
} Resource;

namespace myGA
{
    class GA
    {
    private: //聲明為私有的類成員只能由類的成員函數和好友(類或函數)使用。
        vector<Resource> resource;
        vector<vector<Task>> all_task;

        d1d Crent;                     //雲端資源租用成本
        d2d Ctrans, Ttrans, TDataSize; //資源通訊成本、任務檔案大小
        d3d Texe;                      //執行時間
        int num_machine;               //機器數量
        i1d num_task;                  //子任務數量

        int num_run;
        int num_evaluation;
        // int num_evaluation; //0822
        char cost_time;
        int num_chromosome;
        double crossover_rate;
        double mutation_rate;

        i1d best_sol;
        double Best_score;
        int now_evaluation;
        int now_record;
        int now_run;
        int count_evaluation;

        i1d now_sol;
        double now_score;
        i2d chromosome;
        d1d chromosome_score;
        double similarity;

    protected: //聲明為受保護的類成員可以由類的成員函數和好友(類或函數)使用。 此外，類別所衍生的類別也可以使用這些類別成員。
        int pro_x;
        int pro_y;

    public: //聲明為公共的類成員可以由任何函數使用。
        vector<double> before_score;
        double overall_best;
        f1d iter_best;
        f1d eval_best;
        d1d evaluation_score;
        d1d run_best_score;
        double time1 = 0;
        double time2 = 0;
        double time3 = 0;
        double time4 = 0;
        double time5 = 0;
        double time6 = 0;
        double time7 = 0;
        // float avg_score;
        // i1d __best_obj_vec;

        //--------------------------------------------------------
        GA(int evaluation, int runtime, char cost_or_time, int n_chrom, double cross_rate, double mut_ratef1d, d1d Crent, d2d Ctrans, d2d Ttrans, d2d TDataSize, d3d Texe, int num_machine, i1d num_task); //在定義類別時，您可以使用建構函式（Constructor）來進行物件的初始化。//在物件釋放資源之前，您也可以使用「解構函式」（Destructor）來進行一些善後的工作，例如清除動態配置的記憶體，或像是檔案的儲存、記錄檔的撰寫等等。
        void run();
        void reset_data();
        void initialize();
        i1d create_ans(d1d Crent, d2d Ctrans, d2d TDataSize, d3d Texe);
        double evaluation(i1d sol);
        double evaluation_cost(i1d sol);
        i2d selection(i2d chromosome, d1d chromosome_score);
        i2d crossover(i2d chromosome, double rate);
        i1d find_order(i1d half_sol, i1d sol);
        i2d mutation(i2d chromosome, double rate);
        i1d transform(i1d now_sol);
        bool determination(double now_score, i1d sol);
        double calculate_similarity(i2d chromosome);
        i1d two_opt(i1d sol, int point1, int point2);

        // void print_i1d(i1d v);
        void print_sol(i2d chromosome, int index = 0);
        void print_score(d1d chromosome_score);
    };
} // namespace myGA

#endif