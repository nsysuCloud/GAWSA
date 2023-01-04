#[./file] [evaluation] [runtime] [cost_or_time] [num_chromosome] [crossover_rate] [mutation_rate] [result_file]
./GAWSA.out 15000000 5 t 40 0.6 0.6 DataSet_20m50t.csv GA_DS1_time >> output_DS1_time.txt &
./GAWSA.out 15000000 5 t 40 0.6 0.6 DataSet_20m100t.csv GA_DS2_time >> output_DS2_time.txt &
./GAWSA.out 15000000 5 t 40 0.6 0.6 DataSet_30m100t.csv GA_DS3_time >> output_DS3_time.txt &
./GAWSA.out 15000000 5 t 40 0.6 0.6 DataSet_30m150t.csv GA_DS4_time >> output_DS4_time.txt &
./GAWSA.out 15000000 5 t 40 0.6 0.6 DataSet_30m200t.csv GA_DS5_time >> output_DS5_time.txt &

./GAWSA.out 15000000 5 c 40 0.6 0.6 DataSet_20m50t.csv GA_DS1_cost >> output_DS1_cost.txt &
./GAWSA.out 15000000 5 c 40 0.6 0.6 DataSet_20m100t.csv GA_DS2_cost >> output_DS2_cost.txt &
./GAWSA.out 15000000 5 c 40 0.6 0.6 DataSet_30m100t.csv GA_DS3_cost >> output_DS3_cost.txt &
./GAWSA.out 15000000 5 c 40 0.6 0.6 DataSet_30m150t.csv GA_DS4_cost >> output_DS4_cost.txt &
./GAWSA.out 15000000 5 c 40 0.6 0.6 DataSet_30m200t.csv GA_DS5_cost >> output_DS5_cost.txt &
