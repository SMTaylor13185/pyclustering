#include "support.h"

std::vector<std::vector<double> > * read_sample(const char * const path_file) {
	std::string filename(path_file);

	std::ifstream file_stream(filename.c_str(), std::fstream::in);
	if (!file_stream.is_open()) {
		return NULL;
	}

	std::string line;

	std::vector<std::vector<double> > * dataset = new std::vector<std::vector<double> >();

	while(std::getline(file_stream, line)) {
		std::istringstream parser(line);
		std::vector<double> point;

		double coordinate = 0.0;
		while(parser >> coordinate) {
			point.push_back(coordinate);
		}

		dataset->push_back(point);
	}

	file_stream.close();

	return dataset;
}


std::vector<std::vector<double> > * read_sample(const data_representation * const sample) {
	std::vector<std::vector<double> > * dataset = new std::vector<std::vector<double> >();

	for (unsigned int index = 0; index < sample->size; index++) {
		std::vector<double> point;
		for (unsigned int dimension = 0; dimension < sample->dimension; dimension++) {
			point.push_back(sample->objects[index][dimension]);
		}

		dataset->push_back(point);
	}

	return dataset;
}


clustering_result * create_clustering_result(const std::vector<std::vector<unsigned int> *> * const clusters) {
	clustering_result * result = new clustering_result();

	if (clusters == NULL) {
		result->size = 0;
		result->clusters = NULL;
	}
	else {
		result->size = clusters->size();
		result->clusters = new cluster_representation[result->size];
	}

	for (unsigned int index_cluster = 0; index_cluster < result->size; index_cluster++) {
		result->clusters[index_cluster].size = (*clusters)[index_cluster]->size();
		if (result->clusters[index_cluster].size > 0) {
			result->clusters[index_cluster].objects = new unsigned int[result->clusters[index_cluster].size];

			for (unsigned int index_object = 0; index_object < result->clusters[index_cluster].size; index_object++) {
				result->clusters[index_cluster].objects[index_object] = (*(*clusters)[index_cluster])[index_object];
			}
		}
		else {
			result->clusters[index_cluster].objects = NULL;
		}
	}

	return result;
}


std::vector<differential_result> * rk4(double (*function_pointer)(const double t, const double val, const std::vector<void *> & argv), double initial_value, const double a, const double b, const unsigned int steps, const std::vector<void *> & argv) {
	const double step = (b - a) / (double) steps;

	differential_result current_result;
	current_result.time = 0.0;
	current_result.value = initial_value;

	std::vector<differential_result> * result = new std::vector<differential_result>(1, current_result);

	for (unsigned int i = 0; i < steps; i++) {
		double k1 = step * function_pointer(current_result.time, current_result.value, argv);
		double k2 = step * function_pointer(current_result.time + step / 2.0, current_result.value + k1 / 2.0, argv);
		double k3 = step * function_pointer(current_result.time + step / 2.0, current_result.value + k2 / 2.0, argv);
		double k4 = step * function_pointer(current_result.time + step, current_result.value + k3, argv);

		current_result.value += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
		current_result.time += step;

		result->push_back(current_result);
	}

	return result;
}