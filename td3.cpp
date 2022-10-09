#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    double* a = new double[new_size];
    for (int i=0; i<length; i++) {
        a[i] = array[i];
    }
    for (int i=0; i<new_size-length; i++) {
        a[i+length] = 0;
    }
    delete[] array;
  return a; // YOU CAN CHANGE THIS
}

double* shrink_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    double* a = new double[new_size];
    for (int i=0; i<new_size; i++) {
        a[i] = array[i];
    }
    delete[] array;
  return a; // YOU CAN CHANGE THIS
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    if (current_size == max_size) {
        array = extend_array(array, current_size, current_size+5);
        max_size += 5;
    }
    current_size += 1;
    array[current_size-1] = element;

  return array; // YOU CAN CHANGE THIS
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
//    array[current_size-1] = 0;
    current_size -= 1;
    if (max_size - current_size >= 5) {
        array = shrink_array(array, max_size, max_size-5);
        max_size -= 5;
    }

  return array; // YOU CAN CHANGE THIS
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
    telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
  }

  return hit_target;
}
void printarr(double* arr, int len){
    for (int i = 0; i < len; i++){
        std::cout << arr[i] << " ";
    }
    std::cout << endl;
}

void swap(double *target, int i, int j){
    double tmp = target[i];
    target[i] = target[j];
    target[j] = tmp;
}

void sort(double *targets, const int num_targets) {
  // IMPLEMENT YOUR CODE HERE
    for (int i=0; i<num_targets; i++) {
        int m = 3*i;
        for (int j=i+1; j<num_targets; j++) {
            if (targets[3*j] < targets[m]) {
                m = 3*j;
            }
        }
        swap(targets, m, 3*i);
        swap(targets, m+1, 3*i+1);
        swap(targets, m+2, 3*i+2);

    }
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    int tot_point = 0;
    for (int i=0; i<tot_telemetries; i++) {
        tot_point += telemetries_sizes[i];
        for (int j=0; j<telemetries_sizes[i]; j++) {
            global_telemetry = append_to_array(telemetries[i][j], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
        }
    }
    printarr(global_telemetry, tot_telemetries*3);
    sort(global_telemetry, tot_point/3);
    printarr(global_telemetry, tot_telemetries*3);
}
