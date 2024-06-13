#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include "Movie.h"
#include <map>
#include <cmath>

//Define typedef of map of movies and rates
typedef std::map<sp_movie, double> pred_map;

/**
 * Struct for the comparator between two movies
 */
struct comp_func
{
    bool operator() (const sp_movie &lhs, const sp_movie &rhs) const
    {
      return *lhs < *rhs;
    }
};

// Define typedef for a map of movies and their features.
typedef std::map<sp_movie, std::vector<double>, comp_func> movie_map;

class RecommenderSystem
{
 public:

  explicit RecommenderSystem ();

  /**
   * adds a new movie to the system
   * @param name name_ of movie
   * @param year year_ it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year,
             const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest score based on
   * movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score based
   * on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf procedure
   * with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name_ of movie
   * @param year year_ movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  friend std::ostream &
  operator<< (std::ostream &os, const RecommenderSystem &rs);

 private:
  movie_map movie_map_;

  /**
   * a function that calculates the norm map
   * @param map map of rank
   * @param user a user
   * @return the norm map
   */
  static rank_map calculate_the_norm_map (rank_map map, const RSUser &user);

  /**
   * a function that update the prediction map
   * @param map prediction map
   */
  static pred_map update_prediction (pred_map map);

  /**
   * a function that calculates the multiplication of vector and scalar
   * @param vector vector to multiply
   * @param num scalar
   * @return vector after multiplication
   */
  static std::vector<double>
  mol_vec (const std::vector<double> &vector, double num);

  /**
   * a function that calculates the sum of two vectors
   * @param first_vec a vector
   * @param second_vec a vector
   * @return the sum of the two vectors
   */
  static std::vector<double>
  sum_vectors (std::vector<double> &first_vec, const std::vector<double>
  &second_vec);

  /**
   * a function that calculates the norm of two vectors
   * @param first_vec a vector
   * @param second_vec a vector
   * @return the norm of the vector
   */
  static double
  norm_and_dot (const std::vector<double> &first_vec,
                const std::vector<double> &second_vec);
};

#endif //RECOMMENDERSYSTEM_H