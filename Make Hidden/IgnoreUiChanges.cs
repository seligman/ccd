using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MakeHidden
{
    public class IgnoreUiChanges
    {
        int m_ref = 0;

        public bool Set
        {
            get
            {
                return m_ref > 0;
            }
        }

        public Helper Updating
        {
            get
            {
                return new Helper(this);
            }
        }

        public class Helper : IDisposable
        {
            IgnoreUiChanges m_parent;

            public Helper(IgnoreUiChanges parent)
            {
                m_parent = parent;
                m_parent.m_ref++;
            }

            public void Dispose()
            {
                m_parent.m_ref--;
            }
        }
    }
}
