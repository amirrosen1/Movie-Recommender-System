#include "RSUser.h"
#include "RecommenderSystem.h"
#define NAME_MOVIE "name: "

RSUser::RSUser (const std::string &user_name, const rank_map &user_rank_map,
                const ptr_to_rs &rs)
{
  this->user_name_ = user_name;
  this->user_rank_map_ = user_rank_map;
  this->rs_ = rs;
}

std::string RSUser::get_name () const
{
  return this->user_name_;
}

void
RSUser::add_movie_to_rs (const std::string &name, int year,
                         const std::vector<double> &features, double rate)
{
  sp_movie movie = this->rs_->add_movie (name, year, features);
  this->user_rank_map_[movie] = rate;
}

rank_map RSUser::get_ranks () const
{
  return this->user_rank_map_;
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return this->rs_->recommend_by_content (*this);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return this->rs_->recommend_by_cf (*this, k);
}

double
RSUser::get_prediction_score_for_movie (const std::string &name,
                                        int year, int k) const
{
  sp_movie movie = this->rs_->get_movie (name, year);
  return this->rs_->predict_movie_score (*this, movie, k);
}

std::ostream &operator<< (std::ostream &os, const RSUser &user)
{
  os << NAME_MOVIE << user.get_name () << "\n" << *user.rs_ << std::endl;
  return os;
}