#ifndef INC_23A_C_C__EX5_MOVIE_H
#define INC_23A_C_C__EX5_MOVIE_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>

#define HASH_START 17

class Movie;

typedef std::shared_ptr<Movie> sp_movie;
typedef std::size_t (*hash_func) (const sp_movie &movie);
typedef bool (*equal_func) (const sp_movie &m1, const sp_movie &m2);
std::size_t sp_movie_hash (const sp_movie &movie);
bool sp_movie_equal (const sp_movie &m1, const sp_movie &m2);

class Movie
{
 public:
  /**
     * constructor
     * @param name: name_ of movie
     * @param year: year_ it was made
     */
  Movie (const std::string &name, int year);

  /**
     * returns the name_ of the movie
     * @return const ref to name_ of movie
     */
  std::string get_name () const;

  /**
   * returns the year_ the movie was made
   * @return year_ movie was made
   */
  int get_year () const;

  /**
   * operator< for two movies
   * @param rhs: right hand side
   * @param lhs: left hand side
   * @return returns true if (lhs.year_) < rhs.year_ or
   * (rhs.year_ == lhs.year_ & lhs.name_ < rhs.name_) else return false
   */
  bool operator< (const Movie &other) const;

  /**
   * operator<< for movie
   * @param os ostream to output info with
   * @param movie movie to output
   */
  friend std::ostream &operator<< (std::ostream &os, const Movie &movie);

 private:
  std::string name_;
  int year_;
};

#endif //INC_23A_C_C__EX5_MOVIE_H