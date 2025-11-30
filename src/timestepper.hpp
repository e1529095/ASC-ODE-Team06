#ifndef TIMERSTEPPER_HPP
#define TIMERSTEPPER_HPP

#include <functional>
#include <exception>

#include "Newton.hpp"


namespace ASC_ode {
  using namespace nanoblas;

  class TimeStepper
  {
  protected:
    std::shared_ptr<NonlinearFunction> m_rhs;
  public:
    TimeStepper(std::shared_ptr<NonlinearFunction> rhs) : m_rhs(rhs) {}
    virtual ~TimeStepper() = default;
    virtual void DoStep(double tau, VectorView<double> y) = 0;
  };

  class ExplicitEuler : public TimeStepper
  {
    Vector<> m_vecf;
  public:
    ExplicitEuler(std::shared_ptr<NonlinearFunction> rhs)
    : TimeStepper(rhs), m_vecf(rhs->dimF()) {}
    void DoStep(double tau, VectorView<double> y) override
    {
      this->m_rhs->evaluate(y, m_vecf);
      y += tau * m_vecf;
    }
  };

  class ImplicitEuler : public TimeStepper
  {
    std::shared_ptr<NonlinearFunction> m_equ;
    std::shared_ptr<Parameter> m_tau;
    std::shared_ptr<ConstantFunction> m_yold;
  public:
    ImplicitEuler(std::shared_ptr<NonlinearFunction> rhs)
    : TimeStepper(rhs), m_tau(std::make_shared<Parameter>(0.0))
    {
      m_yold = std::make_shared<ConstantFunction>(rhs->dimX());
      auto ynew = std::make_shared<IdentityFunction>(rhs->dimX());
      m_equ = ynew - m_yold - m_tau * m_rhs;
    }

 class ImprovedEuler : public TimeStepper
  {
    Vector<> m_vecf;
    Vector<> m_ytilde;
  public:
    ImprovedEuler(std::shared_ptr<NonlinearFunction> rhs)
    : TimeStepper(rhs), m_vecf(rhs->dimF()), m_ytilde(rhs->dimX()) {}
    void DoStep(double tau, VectorView<double> y) override
    {
      this->m_rhs->evaluate(y, m_vecf);
      m_ytilde = y + (tau/2.0) * m_vecf;

      this->m_rhs->evaluate(m_ytilde, m_vecf);
      y += tau * m_vecf;
    }
  };

    void DoStep(double tau, VectorView<double> y) override
    {
      m_yold->set(y);
      m_tau->set(tau);
      NewtonSolver(m_equ, y);
    }
  };


  class CrankNicolson : public TimeStepper
  {
  Vector<> m_rhs_oldval;
  std::shared_ptr<NonlinearFunction> m_equ;
  std::shared_ptr<Parameter> m_tau_half;
  std::shared_ptr<ConstantFunction> m_yold;
  std::shared_ptr<ConstantFunction> m_rhs_old;

  public:
  CrankNicolson(std::shared_ptr<NonlinearFunction> rhs)
    : TimeStepper(rhs),
      m_rhs_oldval(rhs->dimF()),
      m_tau_half(std::make_shared<Parameter>(0.0))
  {
    m_yold = std::make_shared<ConstantFunction>(rhs->dimX());
    m_rhs_old = std::make_shared<ConstantFunction>(rhs->dimF());

    auto ynew = std::make_shared<IdentityFunction>(rhs->dimX());

    m_equ = ynew - m_yold - m_tau_half * (m_rhs + m_rhs_old);
  }

  void DoStep(double tau, VectorView<double> y) override
  {
    m_yold->set(y);
    m_tau_half->set(0.5 * tau);

    m_rhs->evaluate(y, m_rhs_oldval);
    m_rhs_old->set(m_rhs_oldval);

    NewtonSolver(m_equ, y);
  }
  };


}


#endif
