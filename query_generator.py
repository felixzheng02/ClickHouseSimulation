import re
import csv


query_str = 'n140083243052824[label="MergeTreeThread:(24 jobs, execution time: 0.442263805 sec., preparation time: 0.00012449 sec.) (Finished)"]; n140083243053208[label="MergeTreeThread:(24 jobs, execution time: 0.442184051 sec., preparation time: 0.000120698 sec.) (Finished)"]; n140083243072664[label="MergeTreeThread:(24 jobs, execution time: 0.432557352 sec., preparation time: 0.000116791 sec.) (Finished)"]; n140083243071896[label="MergeTreeThread:(24 jobs, execution time: 0.435393166 sec., preparation time: 0.0001338 sec.) (Finished)"]; n140083243073048[label="MergeTreeThread:(24 jobs, execution time: 0.428970604 sec., preparation time: 0.000112809 sec.) (Finished)"]; n140083243071128[label="MergeTreeThread:(24 jobs, execution time: 0.455867218 sec., preparation time: 0.000116222 sec.) (Finished)"]; n140083243074200[label="MergeTreeThread:(24 jobs, execution time: 0.449907149 sec., preparation time: 0.000116495 sec.) (Finished)"]; n140083243074968[label="MergeTreeThread:(24 jobs, execution time: 0.437579223 sec., preparation time: 0.000132888 sec.) (Finished)"]; n140083243076120[label="MergeTreeThread:(24 jobs, execution time: 0.455159666 sec., preparation time: 0.000118569 sec.) (Finished)"]; n140083243076888[label="MergeTreeThread:(24 jobs, execution time: 0.451185972 sec., preparation time: 0.000120739 sec.) (Finished)"]; n140083243075352[label="MergeTreeThread:(24 jobs, execution time: 0.438116385 sec., preparation time: 0.000118917 sec.) (Finished)"]; n140083243193112[label="MergeTreeThread:(24 jobs, execution time: 0.435745443 sec., preparation time: 0.000126984 sec.) (Finished)"]; n140083243193880[label="MergeTreeThread:(24 jobs, execution time: 0.455694788 sec., preparation time: 0.000117936 sec.) (Finished)"]; n140083243194648[label="MergeTreeThread:(24 jobs, execution time: 0.456728928 sec., preparation time: 0.000117755 sec.) (Finished)"]; n140083243195416[label="MergeTreeThread:(24 jobs, execution time: 0.427398092 sec., preparation time: 0.000117087 sec.) (Finished)"]; n140083243196184[label="MergeTreeThread:(16 jobs, execution time: 0.284483574 sec., preparation time: 0.000086584 sec.) (Finished)"]; n140083243196952[label="ExpressionTransform:(23 jobs, execution time: 0.126377506 sec., preparation time: 0.000243704 sec.) (Finished)"]; n140083243198488[label="ExpressionTransform:(23 jobs, execution time: 0.140123703 sec., preparation time: 0.000199694 sec.) (Finished)"]; n140083243199256[label="ExpressionTransform:(23 jobs, execution time: 0.136057948 sec., preparation time: 0.000205126 sec.) (Finished)"]; n140083243200024[label="ExpressionTransform:(23 jobs, execution time: 0.139061558 sec., preparation time: 0.000205022 sec.) (Finished)"]; n140083243200792[label="ExpressionTransform:(23 jobs, execution time: 0.138109085 sec., preparation time: 0.000205596 sec.) (Finished)"]; n140083243201560[label="ExpressionTransform:(23 jobs, execution time: 0.118443403 sec., preparation time: 0.000201029 sec.) (Finished)"]; n140083243202328[label="ExpressionTransform:(23 jobs, execution time: 0.126930492 sec., preparation time: 0.000200406 sec.) (Finished)"]; n140083243203096[label="ExpressionTransform:(23 jobs, execution time: 0.140105615 sec., preparation time: 0.000210826 sec.) (Finished)"]; n140083243203864[label="ExpressionTransform:(23 jobs, execution time: 0.120611846 sec., preparation time: 0.000201152 sec.) (Finished)"]; n140083243147288[label="ExpressionTransform:(23 jobs, execution time: 0.128226732 sec., preparation time: 0.000223899 sec.) (Finished)"]; n140083243148056[label="ExpressionTransform:(23 jobs, execution time: 0.140690379 sec., preparation time: 0.000200715 sec.) (Finished)"]; n140083243148824[label="ExpressionTransform:(23 jobs, execution time: 0.139468713 sec., preparation time: 0.000199638 sec.) (Finished)"]; n140083243149592[label="ExpressionTransform:(23 jobs, execution time: 0.130333627 sec., preparation time: 0.000200205 sec.) (Finished)"]; n140083243150360[label="ExpressionTransform:(23 jobs, execution time: 0.12618067 sec., preparation time: 0.000194808 sec.) (Finished)"]; n140083243151128[label="ExpressionTransform:(23 jobs, execution time: 0.140841474 sec., preparation time: 0.000214029 sec.) (Finished)"]; n140083243151896[label="ExpressionTransform:(15 jobs, execution time: 0.085504888 sec., preparation time: 0.000139743 sec.) (Finished)"]; n140083242895576[label="StrictResize:(0 jobs, execution time: 0 sec., preparation time: 0.008430735 sec.) (Finished)"]; n140083242892824[label="AggregatingTransform:(27 jobs, execution time: 0.492036219 sec., preparation time: 0.00015417 sec.) (Finished)"]; n140083243223064[label="AggregatingTransform:(26 jobs, execution time: 0.51315682 sec., preparation time: 0.000151204 sec.) (Finished)"]; n140083243223576[label="AggregatingTransform:(24 jobs, execution time: 0.495604756 sec., preparation time: 0.00013811 sec.) (Finished)"]; n140083243224600[label="AggregatingTransform:(25 jobs, execution time: 0.488858677 sec., preparation time: 0.000154992 sec.) (Finished)"]; n140083243524632[label="AggregatingTransform:(25 jobs, execution time: 0.500719509 sec., preparation time: 0.000136384 sec.) (Finished)"]; n140083243525656[label="AggregatingTransform:(23 jobs, execution time: 0.549811148 sec., preparation time: 0.000128837 sec.) (Finished)"]; n140083243526680[label="AggregatingTransform:(23 jobs, execution time: 0.541007376 sec., preparation time: 0.000125446 sec.) (Finished)"]; n140083243527704[label="AggregatingTransform:(25 jobs, execution time: 0.541104622 sec., preparation time: 0.000182419 sec.) (Finished)"]; n140083243024920[label="AggregatingTransform:(23 jobs, execution time: 0.542520828 sec., preparation time: 0.000129694 sec.) (Finished)"]; n140083243025944[label="AggregatingTransform:(21 jobs, execution time: 0.541358361 sec., preparation time: 0.000126581 sec.) (Finished)"]; n140083243026968[label="AggregatingTransform:(23 jobs, execution time: 0.535772627 sec., preparation time: 0.000127063 sec.) (Finished)"]; n140083243027992[label="AggregatingTransform:(19 jobs, execution time: 0.496303536 sec., preparation time: 0.000121764 sec.) (Finished)"]; n140083243590168[label="AggregatingTransform:(21 jobs, execution time: 0.556993508 sec., preparation time: 0.000132705 sec.) (Finished)"]; n140083243591192[label="AggregatingTransform:(24 jobs, execution time: 0.531906686 sec., preparation time: 0.000132976 sec.) (Finished)"]; n140083243592216[label="AggregatingTransform:(23 jobs, execution time: 0.51497951 sec., preparation time: 0.000155596 sec.) (Finished)"]; n140083243593240[label="AggregatingTransform:(24 jobs, execution time: 0.512390877 sec., preparation time: 0.00014506 sec.) (Finished)"]; n140083242896024[label="Resize:(0 jobs, execution time: 0 sec., preparation time: 0.00022357 sec.) (Finished)"]; n140083243152664[label="ExpressionTransform:(1 jobs, execution time: 0.00033049 sec., preparation time: 0.000014898 sec.) (Finished)"]; n140083243198104[label="ExpressionTransform:(1 jobs, execution time: 0.00032432 sec., preparation time: 0.000015685 sec.) (Finished)"]; n140083243153432[label="ExpressionTransform:(1 jobs, execution time: 0.000343318 sec., preparation time: 0.000014734 sec.) (Finished)"]; n140083243610520[label="ExpressionTransform:(1 jobs, execution time: 0.000294979 sec., preparation time: 0.000013505 sec.) (Finished)"]; n140083243610904[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005768 sec.) (Finished)"]; n140083243611288[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005009 sec.) (Finished)"]; n140083243611672[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004784 sec.) (Finished)"]; n140083243612056[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005018 sec.) (Finished)"]; n140083243612440[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004593 sec.) (Finished)"]; n140083243612824[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004523 sec.) (Finished)"]; n140083243613208[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004548 sec.) (Finished)"]; n140083243613592[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.0000044 sec.) (Finished)"]; n140083243613976[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004756 sec.) (Finished)"]; n140083243614360[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004528 sec.) (Finished)"]; n140083243614744[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004568 sec.) (Finished)"]; n140083243615128[label="ExpressionTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005724 sec.) (Finished)"]; n140083242748312[label="PartialSortingTransform:(1 jobs, execution time: 0.000604579 sec., preparation time: 0.000016181 sec.) (Finished)"]; n140083242750232[label="PartialSortingTransform:(1 jobs, execution time: 0.000561956 sec., preparation time: 0.000017096 sec.) (Finished)"]; n140083242752152[label="PartialSortingTransform:(1 jobs, execution time: 0.000555413 sec., preparation time: 0.000013467 sec.) (Finished)"]; n140083243708440[label="PartialSortingTransform:(1 jobs, execution time: 0.000487269 sec., preparation time: 0.00001164 sec.) (Finished)"]; n140083243710360[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000012928 sec.) (Finished)"]; n140083243712280[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005273 sec.) (Finished)"]; n140083243714200[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004991 sec.) (Finished)"]; n140083243716120[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004925 sec.) (Finished)"]; n140083243718040[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004872 sec.) (Finished)"]; n140083243719960[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004551 sec.) (Finished)"]; n140083243721880[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004631 sec.) (Finished)"]; n140083243723800[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004586 sec.) (Finished)"]; n140083243725720[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005061 sec.) (Finished)"]; n140083243727640[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.00000466 sec.) (Finished)"]; n140083243770520[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005111 sec.) (Finished)"]; n140083243772440[label="PartialSortingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005758 sec.) (Finished)"]; n140083243595288[label="LimitsCheckingTransform:(1 jobs, execution time: 0.000002535 sec., preparation time: 0.000013483 sec.) (Finished)"]; n140083243790360[label="LimitsCheckingTransform:(1 jobs, execution time: 0.000002572 sec., preparation time: 0.000013176 sec.) (Finished)"]; n140083243790872[label="LimitsCheckingTransform:(1 jobs, execution time: 0.000002472 sec., preparation time: 0.000012357 sec.) (Finished)"]; n140083243791384[label="LimitsCheckingTransform:(1 jobs, execution time: 0.000001669 sec., preparation time: 0.000009274 sec.) (Finished)"]; n140083243792920[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004792 sec.) (Finished)"]; n140083243798552[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005136 sec.) (Finished)"]; n140083243800088[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005297 sec.) (Finished)"]; n140083243801624[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004547 sec.) (Finished)"]; n140083243803160[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004564 sec.) (Finished)"]; n140083243804696[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004915 sec.) (Finished)"]; n140083243806232[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004984 sec.) (Finished)"]; n140083243807768[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005034 sec.) (Finished)"]; n140083243809304[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005377 sec.) (Finished)"]; n140083242811416[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005072 sec.) (Finished)"]; n140083242812952[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000005588 sec.) (Finished)"]; n140083242814488[label="LimitsCheckingTransform:(0 jobs, execution time: 0 sec., preparation time: 0.000004474 sec.) (Finished)"]; n140083243774360[label="MergeSortingTransform:(3 jobs, execution time: 0.000155164 sec., preparation time: 0.000017982 sec.) (Finished)"]; n140083243775000[label="MergeSortingTransform:(3 jobs, execution time: 0.000173795 sec., preparation time: 0.000023172 sec.) (Finished)"]; n140083243776280[label="MergeSortingTransform:(3 jobs, execution time: 0.000154127 sec., preparation time: 0.000018357 sec.) (Finished)"]; n140083243777560[label="MergeSortingTransform:(3 jobs, execution time: 0.000131494 sec., preparation time: 0.000015336 sec.) (Finished)"]; n140083243778840[label="MergeSortingTransform:(1 jobs, execution time: 0.000053294 sec., preparation time: 0.000009053 sec.) (Finished)"]; n140083243780120[label="MergeSortingTransform:(1 jobs, execution time: 0.000040705 sec., preparation time: 0.00000942 sec.) (Finished)"]; n140083243781400[label="MergeSortingTransform:(1 jobs, execution time: 0.000034063 sec., preparation time: 0.000009096 sec.) (Finished)"]; n140083243782680[label="MergeSortingTransform:(1 jobs, execution time: 0.000031084 sec., preparation time: 0.000007816 sec.) (Finished)"]; n140083243783960[label="MergeSortingTransform:(1 jobs, execution time: 0.000041455 sec., preparation time: 0.000009014 sec.) (Finished)"]; n140083243785240[label="MergeSortingTransform:(1 jobs, execution time: 0.000045491 sec., preparation time: 0.000010597 sec.) (Finished)"]; n140083243786520[label="MergeSortingTransform:(1 jobs, execution time: 0.000044141 sec., preparation time: 0.000008863 sec.) (Finished)"]; n140083243787800[label="MergeSortingTransform:(1 jobs, execution time: 0.000037025 sec., preparation time: 0.000008672 sec.) (Finished)"]; n140083243789080[label="MergeSortingTransform:(1 jobs, execution time: 0.000040997 sec., preparation time: 0.000010492 sec.) (Finished)"]; n140083243835416[label="MergeSortingTransform:(1 jobs, execution time: 0.000047207 sec., preparation time: 0.000008142 sec.) (Finished)"]; n140083243836696[label="MergeSortingTransform:(1 jobs, execution time: 0.000035786 sec., preparation time: 0.000008491 sec.) (Finished)"]; n140083243837976[label="MergeSortingTransform:(1 jobs, execution time: 0.000029585 sec., preparation time: 0.00000781 sec.) (Finished)"]; n140083243077656[label="MergingSortedTransform:(5 jobs, execution time: 0.000553071 sec., preparation time: 0.000255039 sec.) (Finished)"]; n140083243615512[label="ExpressionTransform:(4 jobs, execution time: 0.000704555 sec., preparation time: 0.000021416 sec.) (Finished)"]; n140083242892312[label="LimitsCheckingTransform:(4 jobs, execution time: 0.00001295 sec., preparation time: 0.000022433 sec.) (Finished)"]; n140083242624408[label="NullSource:(1 jobs, execution time: 0.000001526 sec., preparation time: 0.000003757 sec.) (Finished)"]; n140083242624792[label="NullSource:(1 jobs, execution time: 4.54e-7 sec., preparation time: 0.000002728 sec.) (Finished)"]; n140085332421656[label="LazyOutputFormat:(5 jobs, execution time: 0.000032174 sec., preparation time: 0.000024465 sec.) (Finished)"]; n140083200991256[label="ConvertingAggregatedToChunksTransform:(253 jobs, execution time: 0.003854113 sec., preparation time: 0.014418539 sec.) (Finished)"]; n140085300052248[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020917458 sec., preparation time: 0.000066239 sec.) (Finished)"]; n140085300052632[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020424802 sec., preparation time: 0.00007548 sec.) (Finished)"]; n140085265133592[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020719136 sec., preparation time: 0.000068765 sec.) (Finished)"]; n140085265134744[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020811654 sec., preparation time: 0.000070254 sec.) (Finished)"]; n140085302243352[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020723466 sec., preparation time: 0.000070969 sec.) (Finished)"]; n140085302244504[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020706503 sec., preparation time: 0.000070629 sec.) (Finished)"]; n140084983090968[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020264385 sec., preparation time: 0.000078014 sec.) (Finished)"]; n140084983091352[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020636749 sec., preparation time: 0.000070334 sec.) (Finished)"]; n140083385155608[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020542141 sec., preparation time: 0.000077128 sec.) (Finished)"]; n140083385156760[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020731828 sec., preparation time: 0.000074365 sec.) (Finished)"]; n140085506572824[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.021671374 sec., preparation time: 0.000072962 sec.) (Finished)"]; n140085506573208[label="ConvertingAggregatedToChunksWithMergingSource:(18 jobs, execution time: 0.021622453 sec., preparation time: 0.000090081 sec.) (Finished)"]; n140085315831832[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020443341 sec., preparation time: 0.000070714 sec.) (Finished)"]; n140085315832216[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.019909248 sec., preparation time: 0.000069769 sec.) (Finished)"]; n140085315832600[label="ConvertingAggregatedToChunksWithMergingSource:(16 jobs, execution time: 0.020118892 sec., preparation time: 0.000072979 sec.) (Finished)"]; n140085315832984[label="ConvertingAggregatedToChunksWithMergingSource:(17 jobs, execution time: 0.020511975 sec., preparation time: 0.000068936 sec.) (Finished)"];'
queries = query_str.split('; ')
result = []

for query in queries:
        query_result = []
        query_result.append(query.split('[')[0])
        query_result.append(re.search('\((.*)\sjobs', query).group(1))
        query_result.extend(re.findall(r'\d+\.\d+', query))
        if len(query_result) > 3:
            query_result.insert(2, float(query_result[2])+float(query_result[3]))
        result.append(query_result)

# with open('query_generator.csv', mode='w', newline='') as query_generator:
#     query_generator_writer = csv.writer(query_generator)
#     query_generator_writer.writerows(result)

query = []
block_1 = []
for i in range(0, 16):
     phase = []
     phase.append(round((float(result[i][1])+float(result[i+16][1]))/2))
     phase.append(float(result[i][2])*int(result[i][1])+float(result[i+16][2])*int(result[i+16][1]))
     block_1.append(phase)
query.append(block_1)

block_2 = [[int(result[40][1]), float(result[40][2])*int(result[40][1])]]
for i in range(33, 49):
     phase = []
     phase.append(int(result[i][1]))
     phase.append(float(result[i][2])*int(result[i][1]))
     block_2.append(phase)
query.append(block_2)

block_3 = []
for i in range(50, 66):
     phase = []
     phase.append(max(1, round((float(result[i][1])+float(result[i+16][1])+float(result[i+32][1])+float(result[i+48][1]))/4)))
     phase.append(float(result[i][2])*int(result[i][1])+float(result[i+16][2])*int(result[i+16][1])+float(result[i+32][2])*int(result[i+32][1])+float(result[i+48][2])*int(result[i+48][1]))
     block_3.append(phase)
query.append(block_3)

block_4 = [[int(result[114][1]), float(result[114][2])*int(result[114][1])]]
query.append(block_4)

block_5 = [[int(result[115][1]), float(result[115][2])*int(result[115][1])]]
query.append(block_5)

block_6 = []
for i in range(116, 119):
     phase = []
     phase.append(int(result[i][1]))
     phase.append(float(result[i][2])*int(result[i][1]))
     block_6.append(phase)
query.append(block_6)

block_7 = [[int(result[119][1]), float(result[119][2])*int(result[119][1])]]
query.append(block_7)

block_8 = []
for i in range(121, 137):
     phase = []
     phase.append(int(result[i][1]))
     phase.append(float(result[i][2])*int(result[i][1]))
     block_8.append(phase)
query.append(block_8)

block_9 = [[int(result[120][1]), float(result[120][2])*int(result[120][1])]]
query.append(block_9)

with open('query.csv', mode='w', newline='') as query_file:
    query_writer = csv.writer(query_file)
    query_writer.writerows(query)

print(query)

code = ''
for i in range (0, len(query)):
     for phase in query[i]:
          code += ('phases_' + str(i) + '.push_back(Phase {' + str(phase[0]) + ', ' + str(phase[1]) + '});\n')

print(code)