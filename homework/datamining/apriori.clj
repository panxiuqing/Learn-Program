(ns apriori-ns
  (:require clojure.contrib.combinatorics))

;; Data for testing
(def data {:T1 #{"I1" "I2" "I3" "I5"},
           :T2 #{"I2" "I4"},
           :T3 #{"I2" "I3"},
           :T4 #{"I1" "I2" "I4"},
           :T5 #{"I1" "I3"},
           :T6 #{"I1" "I2" "I3"}})

;; minimal support
(def mini-sup-rate 0.5)
  
(defn appear-number-in-transations
  "Calculate the number of specified set appear in transation"
  [data set-of-l]
  (if (= data nil)
    0
    (+ (if (clojure.set/subset? set-of-l (val (first data))) 1 0)
       (appear-number-in-transations (next data) set-of-l))))

(defn generate-next-c-from-l
  "generate C[n+1] from L[n]"
  [l1 l2]
  (cond (= l1 #{}) nil
        (= (next l1) nil) #{(clojure.set/union (first l1) (first l2))}
        (= (next l2) nil) (conj 
                           (generate-next-c-from-l (next l1) (next l1)))
                           (clojure.set/union (first l1) (first l2))
        :else (conj 
                (generate-next-c-from-l l1 (next l2))
                (clojure.set/union (first l1) (first l2)))))

;; Function implement aprior algrithm
(defn aprior
  ""
  [data mini-sup l]
  (if (= l nil)
    #{}
    (let [l1 (filter #(>= (appear-number-in-transations %)) l)]
      (conj (aprior data mini-sup (generate-next-c-from-l l l) (set l1))))))

(defn main
  ""
  [data mini-sup-rate]
  (let [mini-sup (* mini-sup-rate (count data))
        l (reduce conj #{} (reduce #(clojure.set/union (val %)) data))]
    (aprior [data mini-sup l])))
