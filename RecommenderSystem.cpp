#include "RecommenderSystem.h"
#include "Movie.h"
#define MIN_VALUE (-1)

RecommenderSystem::RecommenderSystem ()
= default;

sp_movie
RecommenderSystem::add_movie (const std::string &name, int year,
                              const std::vector<double> &features)
{
  sp_movie new_movie = std::make_shared<Movie> (name, year);
  movie_map_[new_movie] = features;
  return new_movie;
}

sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  sp_movie new_movie = nullptr;
  rank_map new_rank_map = user.get_ranks ();
  new_rank_map = calculate_the_norm_map (new_rank_map, user);
  std::vector<double> initial_vector;
  for (const auto &movie_feature: movie_map_)
  {
    if (new_rank_map[movie_feature.first] != 0)
    {
      std::vector<double> temp_vector;
      temp_vector = mol_vec (movie_feature.second,
                             new_rank_map[movie_feature.first]);
      initial_vector = sum_vectors (initial_vector, temp_vector);
    }
  }
  double prediction = MIN_VALUE;
  for (auto &movie_feature: movie_map_)
  {
    if (new_rank_map[movie_feature.first] == 0)
    {
      double norm = norm_and_dot (initial_vector, movie_feature.second);
      if (norm > prediction)
      {
        prediction = norm;
        new_movie = movie_feature.first;
      }
    }
  }
  return new_movie;
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  sp_movie chosen_movie = nullptr;
  double prediction = 0;
  rank_map user_rank = user.get_ranks ();
  for (const auto &movie_rank: user_rank)
  {
    if (movie_rank.second == 0)
    {
      double temp = predict_movie_score (user, movie_rank.first, k);
      if (temp > prediction)
      {
        prediction = temp;
        chosen_movie = movie_rank.first;
      }
    }
  }
  return chosen_movie;
}

double
RecommenderSystem::predict_movie_score (const RSUser &user,
                                        const sp_movie &movie, int k)
{
  double final_prediction;
  pred_map prediction;
  std::vector<double> temp;
  rank_map user_rank = user.get_ranks ();
  for (const auto &movie_feature: movie_map_)
  {
    if (movie_feature.first->get_name () != movie->get_name () &&
        (user_rank[movie_feature.first]) != 0)
    {
      double norm = norm_and_dot (movie_feature.second, movie_map_[movie]);
      double result = norm;
      prediction[movie_feature.first] = result;
      if ((int) prediction.size () > k)
      {
        prediction = update_prediction (prediction);
      }
    }
  }
  double sum = 0;
  double prediction_sum = 0;
  for (const auto &prediction_num: prediction)
  {
    sum += user_rank[prediction_num.first] * prediction_num.second;
    prediction_sum += prediction_num.second;
  }
  final_prediction = sum / prediction_sum;
  return final_prediction;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  sp_movie new_movie = std::make_shared<Movie> (name, year);
  if (movie_map_.find (new_movie) != movie_map_.end ())
  {
    return movie_map_.find (new_movie)->first;
  }
  return nullptr;
}

std::ostream &operator<< (std::ostream &os, const RecommenderSystem &rs)
{

  for (const auto &entry: rs.movie_map_)
  {
    os << *entry.first;
  }
  return os;
}

rank_map
RecommenderSystem::calculate_the_norm_map (rank_map map, const RSUser &user)
{
  double score_sum = 0;
  int count_of_movie = 0;
  for (const auto &movie: user.get_ranks ())
  {
    if (movie.second != 0)
    {
      score_sum += movie.second;
      count_of_movie++;
    }
  }
  double avg_score = score_sum / count_of_movie;
  for (auto &movie: map)
  {
    if (movie.second != 0)
    {
      movie.second = movie.second - avg_score;
    }
  }
  return map;
}

pred_map RecommenderSystem::update_prediction (pred_map prediction)
{
  double min = 1;
  sp_movie min_movie = nullptr;
  for (const auto &prediction_num: prediction)
  {
    if (prediction_num.second < min)
    {
      min = prediction_num.second;
      min_movie = prediction_num.first;
    }
  }
  prediction.erase (min_movie);
  return prediction;

}

std::vector<double>
RecommenderSystem::mol_vec (const std::vector<double> &vector, double num)
{
  std::vector<double> vec;
  for (const double i: vector)
  {
    vec.push_back (i * num);
  }
  return vec;
}

std::vector<double>
RecommenderSystem::sum_vectors (std::vector<double> &first_vec,
                                const std::vector<double> &second_vec)
{
  if (first_vec.empty ())
  {
    first_vec = second_vec;
  }
  else
  {
    for (size_t i = 0; i < first_vec.size (); i++)
    {
      first_vec[i] += second_vec[i];
    }
  }
  return first_vec;
}

double
RecommenderSystem::norm_and_dot (const std::vector<double> &first_vec,
                                 const std::vector<double> &second_vec)
{
  double sum = 0;
  double norm1 = 0;
  double norm2 = 0;
  for (size_t i = 0; i < first_vec.size (); ++i)
  {
    sum += first_vec[i] * second_vec[i];
    norm1 += first_vec[i] * first_vec[i];
    norm2 += second_vec[i] * second_vec[i];
  }
  return sum / (sqrt (norm1) * sqrt (norm2));
}