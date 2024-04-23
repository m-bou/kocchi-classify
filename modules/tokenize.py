from sklearn.datasets import fetch_olivetti_faces, fetch_20newsgroups
from sklearn.feature_extraction.text import CountVectorizer
tf_vectorizer = CountVectorizer(max_df=0.9, min_df=2, max_features=1000,
                                    stop_words="french")

newsdata = tf_vectorizer.fit_transform(newsgroups.data)