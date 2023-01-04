# BLM4540 Digital Image Processing

Ödev tanımı, rapor ve kodlar ilgili dosyalarda bulunabilir.

### hw1:
**Konu**: Kenar Belirleme İşlemi (C dilinde) <br/>
**Açıklama**: Bu ödevde gri renklerdeki bir resme kenar belirleme işlemi uygulamamız istenmektedir.<br/><br/>
**Fonksiyonlar**: Aşağıdaki işlemleri yapan fonksiyonları yazınız. 
1. *Görüntü dosyasının okunması*: .pgm formatında verilen giriş görüntüsün formatını çözerek, 
görüntüyü iki boyutlu bir matrise yerleştiren fonksiyonu yazınız. 
2. *Görüntüye verilen kernel ile konvolüsyon işlemi uygulanması*: Parametre olarak kernel
matrisinin boyutunu (3x3, 5x5 gibi tek sayılı bir kare matris olabilir) ve değerlerini veriniz. 
3. *Kernel uygulanmış görüntülerin pgm formatında dosyaya yazılması*: Kernel uygulanmış 
sonuç görüntüyü “pgm” formatında yeni bir dosyaya yerleştiren fonksiyonu yazınız.


-----------
### hw2: 
**Konu**: İçerik Tabanlı Görüntü Erişimi (Content Based Image Retrieval)<br/>**Açıklama**: Bu ödevde resimlerin doku benzerliklerini değerlendirerek bir sorgu resmine en çok benzeyen 3 adet resmi tespit etmemiz istenmektedir. Sistemin başarısı ekte paylaşılan 10 doku sınıfına ait örnek resimler için değerlendirilecektir.<br/><br/>
**İşlem Adımları**: Ödev 2 aşamadan oluşmaktadır. 
1. *Veritabanındaki resimlerin hazırlanması*: Aşağıdaki işlemleri train directory’si altındaki bütün resimler için sadece 1 defa yapınız. Elde ettiğiniz sonuçları test resimleri için kullanmak amacıyla saklayınız:<br/> - Doku histogramlarının hesaplanması:<br/> - Doku benzerliklerinin ölçülmesi için resimlerin Uniform-LBP(Local Binary Pattern) histogramlarını hesaplayınız. Histogram sonuçlarını [0,1] aralığına normalize etmeyi unutmayınız. 
2. *Örnek Test Resimleri İle Sistem Başarısının Ölçülmesi*: Test aşamasında test directory’si altında bulunan resimler için aşağıdaki işlemleri yapınız.<br/> 
a. Test resminin Uniform-LBP histogramını eğitim örnekleri için yaptığınız gibi hesaplayınız.<br/>
b. Resimlerin benzerliklerini ölçerken verilen test resminin eğitim resimlerinin hepsine mesafesini Manhattan Mesafesi yöntemi ile hesaplayarak en benzer olduğu 3’er resmi


-----------
### hw3:
**Konu/Açıklama**: Bu ödevde konvolüsyonel sinir ağları ile CIFAR-10 veri kümesi kullanılarak eğitilmiş bir 
öğrenme modelini kullanarak sınıflandırma yapan bir sistem tasarlamamız istenmektedir.

