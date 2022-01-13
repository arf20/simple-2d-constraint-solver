#include "../include/line_constraint.h"

#include <cmath>

atg_scs::LineConstraint::LineConstraint() : Constraint(1, 1) {
    m_local_x = m_local_y = 0.0;
    m_p0_x = m_p0_y = 0.0;
    m_dx = m_dy = 0.0;
    m_ks = 10.0;
    m_kd = 1.0;
}

atg_scs::LineConstraint::~LineConstraint() {
    /* void */
}

void atg_scs::LineConstraint::calculate(
        Output *output,
        int body,
        SystemState *state)
{
    if (body != m_bodies[0]->index) {
        output->n = 0;
        return;
    }

    output->n = 1;

    const double q1 = state->p_x[body];
    const double q2 = state->p_y[body];
    const double q3 = state->theta[body];

    const double cos_q3 = std::cos(q3);
    const double sin_q3 = std::sin(q3);

    const double bodyX = q1 + cos_q3 * m_local_x - sin_q3 * m_local_y;
    const double bodyY = q2 + sin_q3 * m_local_x + cos_q3 * m_local_y;

    const double perpX = -m_dy;
    const double perpY = m_dx;

    const double deltaX = bodyX - m_p0_x;
    const double deltaY = bodyY - m_p0_y;

    const double C = deltaX * perpX + deltaY * perpY;

    const double dC_dq1 = 1.0 * perpX;
    const double dC_dq2 = 1.0 * perpY;
    const double dC_dq3 =
        (-sin_q3 * m_local_x - cos_q3 * m_local_y) * perpX +
        (cos_q3 * m_local_x - sin_q3 * m_local_y) * perpY;

    const double d2C_dq1_2 = 0.0;
    const double d2C_dq2_2 = 0.0;
    const double d2C_dq3_2 =
        (-cos_q3 * m_local_x + sin_q3 * m_local_y) * perpX +
        (-sin_q3 * m_local_x - cos_q3 * m_local_y) * perpY;

    output->dC_dq[0][0] = dC_dq1;
    output->dC_dq[0][1] = dC_dq2;
    output->dC_dq[0][2] = dC_dq3;

    output->d2C_dq2[0][0] = d2C_dq1_2;
    output->d2C_dq2[0][1] = d2C_dq2_2;
    output->d2C_dq2[0][2] = d2C_dq3_2;

    output->ks[0] = m_ks * C;
    output->kd[0] = m_kd;
}