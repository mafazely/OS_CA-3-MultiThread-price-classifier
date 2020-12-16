# OS_CA-3-MultiThread-price-classifier
An OS course project to predict price of phones by train data and weight Matrix


## this project has two implementation:
### serial
### parallel


## How to run ?
For serial part you in `serial` directory just run `make` command to make project by g++.
then run executable file in this form :
```
‫‪./PhonePricePrediction.out‬‬‬ <datasets‬‬ path>
```

like this :
```
‫‪./PhonePricePrediction.out‬‬ ‫‪datasets/
```
And for parallel part you first copy `breaker.sh` scripts to your dataset directory near train.csv and weight.csv files and run it in this form:
```
‫‪./breaker.sh‬‬‬ <Number of split files>
```
that this number must be equal to thread number that defined in main.cpp
Default in this project like this :
```
./breaker.sh 6
```
Then in `parallel` directory just run `make` command to make project by g++.
```
‫‪./PhonePricePrediction.out‬‬‬ <datasets‬‬ path>
```

like this :
```
‫‪./PhonePricePrediction.out‬‬ ‫‪datasets/
```

## note : you can change num of thread by edit `NUMBER_OF_THREADS` define to your custom value in main.cpp file
### desire output format :
```
‫‪Accuracy:‬‬ ‫‪<accuracy_percentage>%‬‬
```
like this :
```
‫‪Accuracy:‬‬ ‫‪93.05%‬‬
```

### dataset format :
in dataset directory you must have two files:
train.csv file
weights.csv file
that train had entries of features and weights file is weight vector for each price range class
[csv files must have header.]



Created by Amin Fazeli on 16 Dec 2020.

OS Course - 
Fall 1399 - 
University of Tehran
