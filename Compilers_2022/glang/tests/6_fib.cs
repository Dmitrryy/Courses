class FibGenerator {
    m_a : Integer;
    m_b : Integer;

    method Init(a : Integer, b : Integer) {
        m_a.Set(a);
        m_b.Set(b);
    }

    method Next() : Integer {
        next : Integer;
        next.Set(m_a.Add(m_b));
        
        tmp_a : Integer;
        tmp_a.Set(m_a);
        m_a.Set(m_b);
        m_b.Set(next);

        return tmp_a;
    }
}

class Program {
    m_fg : FibGenerator;

    method Program() {
        target : Integer;
        target.Set(20);

        m_fg.Init(1, 1);
        while(target) {
            m_fg.Next().Print();

            target.Set(target.Sub(1));
        }
    }
}
