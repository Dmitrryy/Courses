
class Program {
    m_int : Integer;

    method ifTest() {
        // print only 2 times (6)
        m_int.Set(1);
        if(m_int) {
            loc : Integer;
            loc.Set(6);
            loc.Print();
            
            if(m_int) {
                loc.Print();
            }

            if(0) {
                loc.Print();
            }
        }
        // Note: 'loc' is not visible here
    }
    
    method Program() {
        this.ifTest();
    }
}
