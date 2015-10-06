using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;

namespace PddXmlParser
{
    class Program
    {
        static void Main(string[] args)
        {
            XDocument doc;
            using (FileStream fs = File.OpenRead("tests.xml"))
            {
                using (XmlReader xreader = XmlReader.Create(fs))
                {
                    doc = XDocument.Load(xreader);
                }
            }

            XElement root = doc.Element("Exam");
            foreach (XElement t in root.Elements("Test"))
            {
                foreach (XElement q in t.Elements("Question"))
                {
                    XElement cs = q.Element("Choices");
                    XElement a = q.Element("Answer");

                    if (a == null)
                    {
                        XElement na = new XElement(XName.Get("Answer"));
                        na.Value = "NaN";
                        q.Add(na);
                        continue;
                    }

                    string answer = a.Value.Trim();

                    foreach (XElement c in cs.Elements("Choice"))
                    {
                        string id = c.Attribute(XName.Get("ID")).Value;
                        string txt = c.Attribute(XName.Get("Text")).Value;
                        txt = txt.Trim();
                        if (string.Compare(answer, txt) == 0)
                        {
                            a.SetValue(id);
                        }
                    }
                }
            }

            doc.Save("tests2.xml");
        }
    }
}
