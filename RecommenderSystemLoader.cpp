#include "RecommenderSystemLoader.h"
#include "RecommenderSystem.h"
#include "fstream"
#include "string"
#include <stdexcept>
#include <sstream>

#define MIN_NUM 1
#define MAX_NUM 10
#define INVALID_FILE "file is invalid"
#define INVALID_SCORE "score is invalid"

RecommenderSystemPtr
RecommenderSystemLoader::create_rs_from_movies_file (const std::string
                                                     &movies_file_path)
noexcept (false)
{
  std::ifstream movie_and_score (movies_file_path, std::ios::in);
  if (!movie_and_score.is_open ())
  {
    throw std::invalid_argument (INVALID_FILE);
  }
  std::string line;
  RecommenderSystemPtr rs = std::make_unique<RecommenderSystem> ();
  while (std::getline (movie_and_score, line))
  {
    std::stringstream stream_of_stream (line);
    std::string name_of_movie;
    std::string year_of_movie;
    std::vector<double> score_of_movie;
    std::getline (stream_of_stream, name_of_movie, '-');
    std::getline (stream_of_stream, year_of_movie, ' ');
    std::string score;
    while (std::getline (stream_of_stream, score, ' '))
    {
      double score_d = std::stod (score);
      if (score_d < MIN_NUM || score_d > MAX_NUM)
      {
        movie_and_score.close ();
        throw std::invalid_argument (INVALID_SCORE);
      }
      score_of_movie.push_back (score_d);
    }
    rs->add_movie (name_of_movie, std::stoi (year_of_movie), score_of_movie);
  }
  movie_and_score.close ();
  return rs;
}