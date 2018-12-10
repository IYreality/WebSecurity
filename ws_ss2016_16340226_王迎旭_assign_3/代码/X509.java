//导入所需要的包
import java.security.*;
import java.io.*;
import java.security.cert.*;
import java.security.cert.Certificate;
public class X509 {
    public static void main(String args[]) throws IOException {
        //X.509 的 CertificateFactory 返回的证书必须是 java.security.cert.X509Certificate 的实例
        //直接构建Factory类型的对象
        CertificateFactory factory ;
        // 这里我们使用参考文献中的代码样例进行操作
        /* 
            FileInputStream fis = new FileInputStream(filename);
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            Collection c = cf.generateCertificates(fis);
            Iterator i = c.iterator();
            while (i.hasNext()) {
                Certificate cert = (Certificate)i.next();
                System.out.println(cert);
            }
        */
        try
        {
            //文件流读入X509证书
            FileInputStream fis = new FileInputStream("test.cer") ;
            //实例化类型
            factory = CertificateFactory.getInstance("X.509") ;
            // 用文件流读入证书
            Certificate c = factory.generateCertificate(fis);
            //这里需要使用强制类型转换将factory类型转为X509certificate类型，等于类型的向上转换
            X509Certificate t = (X509Certificate)c ;
            //文件读取完毕
            fis.close();
            String s = c.toString() ;
            //整体的X509信息
            System.out.println("输出证书信息: \n" + s ) ;
            //其余输出信息同 1.2 组成结构顺序
            //版本号
            System.out.println("版本号: " + t.getVersion()) ;
            //序列号
            System.out.println("序列号: " + t.getSerialNumber().toString(16)) ;
            //签名算法
            System.out.println("签名算法："+ t.getSigAlgName()) ;
            //颁发者
            System.out.println("颁发者: "+ t.getIssuerDN()) ;
            //证书有效期
            System.out.println("有效起始日期："+ t.getNotBefore()) ;
            System.out.println("有效终止日期："+ t.getNotAfter()) ;
            //主体
            System.out.println("主体名："+ t.getSubjectDN()) ;
            //主题唯一身份信息
            System.out.println("签名："+ t.getSignature().toString()) ;
            PublicKey key = t.getPublicKey() ;
            byte [] KeyEncode = key.getEncoded() ;  
            //公钥
            System.out.println("公钥: ") ;
            for(int i = 0 ; i < KeyEncode.length ; i++)
            {
                System.out.print(KeyEncode[i]+",");
            }
            System.out.println();
        }
        catch (CertificateException e) {
			e.printStackTrace();
		}
    }
};