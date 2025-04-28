# sosyalagproje
Bu projede, kullanıcılar ve onların arkadaşlık ilişkilerini temsil eden bir sosyal ağ modeli kurduk.
Amacımız, bu ağı graf yapısıyla göstermek ama analiz işlemlerini daha verimli yapabilmek için ağaç tabanlı veri yapıları kullanmaktı.
Özellikle de kullanıcıların kimliklerini hızlı aramak için Kırmızı-Siyah Ağaç (Red-Black Tree) kullandık.
Bu sayede kullanıcı aramaları dengeli ve hızlı oldu.
Kısaca:
Graf ➔ Arkadaşlık ilişkileri,
Red-Black Tree ➔ Kimlik bazlı kullanıcı yönetimi görevlerini üstlendi.
 
🛠 Kullanılan Veri Yapıları
1. Graph (Graf Yapısı)
Burada her bir kullanıcı bir GraphNode olarak tutuluyor.
Arkadaşlarını ise bağlı bir liste şeklinde (adjacency list) saklıyoruz.
Şöyle düşünebilirsin:
•	Kullanıcı: 101
•	Arkadaşları: [102, 103] → yani 101'in friends listesinde 102 ve 103 var.
Kullandığımız temel yapılar:
c
KopyalaDüzenle
typedef struct FriendNode {
    int friend_id;
    struct FriendNode* next;
} FriendNode;

typedef struct GraphNode {
    int user_id;
    FriendNode* friends;
    struct GraphNode* next;
} GraphNode;
 
2. Red-Black Tree (Kırmızı-Siyah Ağaç)
Burası işin ağaç kısmı.
Kullanıcı kimliklerini Red-Black Tree'ye ekleyerek dengeli bir ağaç oluşturuyoruz.
Red-Black Tree'nin amacı şuydu:
•	Her zaman neredeyse dengeli kalmak
•	Kullanıcı ekleme ve arama işlemlerini logaritmik zamanda yapmak (O(log n))
Basitleştirilmiş bir RB-Tree kullandık, renk ayarlamaları (rotation) kodda basitleştirildi ama temel fikir korundu.
c
KopyalaDüzenle
typedef struct RBNode {
    int user_id;
    struct RBNode* left;
    struct RBNode* right;
    int color; // 0 = Black, 1 = Red
} RBNode;
 
⚡ Fonksiyonlar Ne İşe Yarıyor?
Projede bol bol yardımcı fonksiyon yazdık. Ana görevleri şöyle toparlayabiliriz:
•	load_data_from_file()
veriseti.txt dosyasını okuyarak kullanıcıları ve arkadaşlık ilişkilerini oluşturuyor.
•	insert_rbtree()
Kullanıcıyı Red-Black Tree’ye ekliyor.
•	create_graph(), add_user(), add_friendship()
Kullanıcıları ve ilişkileri graf üzerinde oluşturuyor.
•	dfs_helper() ve find_friends_at_distance()
Belirli bir kullanıcıdan başlayıp DFS (Depth-First Search) yaparak belirli mesafedeki arkadaşları buluyoruz.
•	find_common_friends()
İki kullanıcının ortak arkadaşlarını listeliyor.
•	calculate_influence()
Bir kullanıcının kaç kişiye ulaşabileceğini hesaplıyor. Yani etki alanını buluyoruz.
•	inorder_traversal()
Red-Black Tree üzerinde sıralı gezinme yapıp kullanıcıları sıralı listeliyoruz.
Hepsini toparladık, menüye koyduk, kullanıcı hangi işlemi isterse onu seçip çalıştırabiliyor.
 
📝 Programın Çalışma Mantığı
Adım adım gidersek:
1.	Program çalışır.
2.	veriseti.txt dosyasını okuruz.
o	"USER" satırlarını okuyunca kullanıcıyı oluştururuz.
o	"FRIEND" satırlarını okuyunca iki kullanıcıyı birbirine bağlarız.
3.	Kullanıcıya menü gösteririz.
4.	Kullanıcı:
o	Arkadaşlarını görebilir,
o	Ortak arkadaşları bulabilir,
o	Belirli mesafedeki kişileri çıkarabilir,
o	Etki alanı analizi yapabilir,
o	Ağdaki tüm kullanıcıları sıralayabilir.
Ve tüm bunlar hem graf hem de ağaç yapısını kullanarak yapılır
