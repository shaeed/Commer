# Generated by Django 2.0.7 on 2018-09-02 02:00

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('infinityroom', '0009_device_devgrp_db'),
    ]

    operations = [
        migrations.AlterField(
            model_name='device',
            name='devGrp_db',
            field=models.TextField(null=True),
        ),
    ]